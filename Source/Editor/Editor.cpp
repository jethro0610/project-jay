#include "Editor.h"
#include "Camera/Camera.h"
#include "Level/LevelLoader.h"
#include "Rendering/Renderer.h"
#include "Systems/Systems.h"
#include "Terrain/Terrain.h"
#include "Collision/Ray.h"
#include <GLFW/glfw3.h>
using namespace glm;

Editor::Editor(
    Camera& camera, 
    EntityManager& entityManager, 
    Inputs& inputs,
    LevelLoader& levelLoader,
    LevelProperties& levelProperties,
    Platform& platform, 
    ResourceManager& resourceManager,
    Renderer& renderer,
    Terrain& terrain,
    bool& running
):
camera_(camera),
entityManager_(entityManager),
inputs_(inputs),
levelLoader_(levelLoader),
levelProperties_(levelProperties),
platform_(platform),
resourceManager_(resourceManager),
renderer_(renderer),
terrain_(terrain),
running_(running),
target_(entityManager),
textInput_(platform),
args_({
    camera, 
    entityManager, 
    inputs, 
    levelLoader, 
    levelProperties, 
    platform, 
    renderer, 
    resourceManager, 
    terrain, 
    modeText_,
    target_, 
    textInput_
}),
#define MODEEXPANSION(TYPE, VAR) VAR(args_),
#define TAILMODEEXPANSION(TYPE, VAR) VAR(args_)
EXPANDMODES 
#undef MODEEXPANSION 
#undef TAILMODEEXPANSION
{
    mode_ = nullptr;
    #define MODEEXPANSION(TYPE, VAR) modes_.push_back(&VAR);
    #define TAILMODEEXPANSION(TYPE, VAR) modes_.push_back(&VAR);
    EXPANDMODES 
    #undef MODEEXPANSION 
    #undef TAILMODEEXPANSION

    active_ = false;

    modeText_.properties_.scale = 40.0f;
    modeText_.properties_.hAlignment = Text::RIGHT_ALIGN;
    modeText_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    modeText_.properties_.hAnchor = Text::RIGHT_ALIGN;
    modeText_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    target_.text_.properties_.scale = 40.0f;
    target_.text_.properties_.position.y -= 40.0f;
    target_.text_.properties_.hAlignment = Text::RIGHT_ALIGN;
    target_.text_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    target_.text_.properties_.hAnchor = Text::RIGHT_ALIGN;
    target_.text_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    textInput_.text_.properties_.scale = 40.0f;
    textInput_.text_.properties_.hAlignment = Text::LEFT_ALIGN;
    textInput_.text_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    textInput_.text_.properties_.hAnchor = Text::LEFT_ALIGN;
    textInput_.text_.properties_.vAnchor = Text::BOTTOM_ALIGN;
}

void Editor::StartEditing() {
    active_ = true;
    target_.Set(NULL_ENTITY);
    SetMode(defaultMode_);
    defaultMode_.SetSubmode(DS_Camera);
    camera_.target_ = NULL_ENTITY;
    ScreenText::SetEnabled(false);
    levelLoader_.ReloadLevel();
}

void Editor::StopEditing() {
    active_ = false;
    target_.Set(NULL_ENTITY);
    camera_.target_ = PLAYER_ENTITY;
    platform_.SetMouseVisible(false);
    SaveLevel();
}

void Editor::SetMode(EditorMode* mode) {
    if (mode_ == mode) return;
    if (mode->requiresTarget_ && target_.Get() == NULL_ENTITY) return;

    if (mode_ != nullptr)
        mode_->OnEnd();

    mode_ = mode;
    FlushInputs();
    mode_->OnStart();
}

void Editor::SetMode(EditorMode& mode) {
    SetMode(&mode);
}

void Editor::Update() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    
    if (platform_.heldKeys_[GLFW_KEY_LEFT_CONTROL] && platform_.pressedKeys_['E']) {
        StopEditing();
        FlushInputs();
        return;
    }

    if (mode_ == &defaultMode_) {
        for (EditorMode* mode : modes_) {
            if (platform_.pressedKeys_[mode->GetBinding()]) {
                SetMode(mode);
                break;
            }
        }
    }
    else if (platform_.pressedKeys_[defaultMode_.GetBinding()]) {
        mode_->OnCancel();
        SetMode(defaultMode_);
    }
    else if (platform_.pressedKeys_[GLFW_KEY_ENTER]) {
        if (mode_->OnConfirm())
            SetMode(defaultMode_);
    }

    mode_->Update();

    TransformSystem::ForceRenderTransforms(
        entityManager_.entities_,
        entityManager_.components_
    );
    entityManager_.DestroyEntities();
    entityManager_.SpawnEntities();
    renderer_.RenderEdit(
        entityManager_.entities_, 
        entityManager_.components_,
        *this,
        levelProperties_,
        terrain_ 
    );
    FlushInputs();
}

// Since editor is PC only this will just be in the regular file
void Editor::FlushInputs() {
    // Have to toggle on flush since released keys are cleared per game update
    // and not per program update. Clearing happens in this function
    if (platform_.pressedKeys_[GLFW_KEY_ESCAPE])
        running_ = false;

    platform_.FlushKeys();
    platform_.gamepad_.pressedButtons_.reset();
    platform_.gamepad_.releasedButtons_.reset();
}

void Editor::SaveLevel() {
    nlohmann::json level;

    level["spread"]["model"] = levelProperties_.spreadModel->DBG_name;
    for (Material* material : levelProperties_.spreadMaterials)
        level["spread"]["materials"].push_back(material->DBG_name);

    level["terrain"]["material"] = levelProperties_.terrainMaterial->DBG_name;
    level["seed"]["material"] = levelProperties_.seedMaterial->DBG_name;
    
    level["blob"]["seed"] = levelProperties_.blob.seed;
    level["blob"]["frequency"] = levelProperties_.blob.frequency;
    level["blob"]["minRadius"] = levelProperties_.blob.minRadius;
    level["blob"]["maxRadius"] = levelProperties_.blob.maxRadius;

    for (int i = 0; i < NoiseLayer::MAX; i++) {
        nlohmann::json noiseLayerData;
        noiseLayerData["active"] = levelProperties_.noiseLayers[i].active;
        noiseLayerData["seed"] = levelProperties_.noiseLayers[i].seed;
        noiseLayerData["frequency"]["x"] = levelProperties_.noiseLayers[i].frequency.x;
        noiseLayerData["frequency"]["y"] = levelProperties_.noiseLayers[i].frequency.y;
        noiseLayerData["multiplier"] = levelProperties_.noiseLayers[i].multiplier;
        noiseLayerData["exponent"] = levelProperties_.noiseLayers[i].exponent;
        level["noise_layers"][i] = noiseLayerData;
    }
    
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager_.entities_[i];
        if (!entity.alive_) continue;
        
        nlohmann::json entityData; 
        entityData["name"] = entity.DBG_name;
        Transform& transform = transformComponent.transform[i];

        entityData["transform"]["position"]["x"] = transform.position.x;
        entityData["transform"]["position"]["y"] = transform.position.y;
        entityData["transform"]["position"]["z"] = transform.position.z;

        entityData["transform"]["scale"]["x"] = transform.scale.x;
        entityData["transform"]["scale"]["y"] = transform.scale.y;
        entityData["transform"]["scale"]["z"] = transform.scale.z;

        glm::vec3 eulerRotation = glm::eulerAngles(transform.rotation);
        entityData["transform"]["rotation"]["x"] = eulerRotation.x;
        entityData["transform"]["rotation"]["y"] = eulerRotation.y;
        entityData["transform"]["rotation"]["z"] = eulerRotation.z;

        level["entities"].push_back(entityData);
    }
    std::ofstream assetLevelFile("../Assets/levels/" + levelLoader_.DBG_currentLevel + ".json");
    assetLevelFile << std::setw(4) << level << std::endl;
    assetLevelFile.close();

    std::ofstream workingLevelFile("levels/" + levelLoader_.DBG_currentLevel + ".json");
    workingLevelFile<< std::setw(4) << level << std::endl;
    workingLevelFile.close();
    DEBUGLOG("Saved level: " << levelLoader_.DBG_currentLevel);
}
