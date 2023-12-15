#include "Editor.h"
#include "Rendering/Renderer.h"
#include "Systems/Systems.h"
#include <GLFW/glfw3.h>
using namespace glm;

Editor::Editor(
    Camera& camera, 
    EntityManager& entityManager, 
    Inputs& inputs,
    LevelLoader& levelLoader,
    LevelProperties& levelProperties,
    Platform& platform, 
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
renderer_(renderer),
terrain_(terrain),
running_(running)
{
    active_ = false;
    target_ = NULL_ENTITY;
    mode_ = EM_Camera;
}

void Editor::StartEditing() {
    active_ = true;
    target_ = NULL_ENTITY;
    mode_ = EM_Camera;
    camera_.target_ = NULL_ENTITY;
    ScreenText::SetEnabled(false);
    levelLoader_.LoadLevel(levelLoader_.DBG_currentLevel);

    target_ = 1;
}

void Editor::StopEditing() {
    active_ = false;
    target_ = NULL_ENTITY;
    camera_.target_ = PLAYER_ENTITY;
    platform_.SetMouseVisible(false);
    ScreenText::Clear();
    ScreenText::SetEnabled(false);
    SaveLevel();
}

void Editor::SetMode(EditorMode mode) {
    mode_ = mode;
    switch (mode_) {
        case EM_Camera: {
            platform_.SetMouseVisible(false); 
            break;
        }

        case EM_Mouse: {
            platform_.SetMouseVisible(true); 
            break;
        }
        
        case EM_PlanarMove: {
            platform_.SetMouseVisible(false); 
            break;
        }

        default:
            break;
    }
}

void Editor::Update() {
    if (platform_.pressedKeys_['1'])
        SetMode(EM_Camera);
    if (platform_.pressedKeys_['2'])
        SetMode(EM_Mouse);
    if (platform_.pressedKeys_['3'])
        SetMode(EM_PlanarMove);
    if (platform_.heldKeys_[GLFW_KEY_LEFT_CONTROL] && platform_.pressedKeys_['E'])
        StopEditing();

    switch (mode_) {
        case EM_Camera: {
            CameraUpdate();
            break;
        }

        case EM_Mouse: {
            MouseUpdate();
            break;
        }

        case EM_PlanarMove: {
            PlanarMoveUpdate();
            break;
        }

        default:
            break;
    }

    TransformSystem::ForceRenderTransforms(
        entityManager_.entities_,
        entityManager_.components_
    );
    renderer_.RenderMinimal(
        entityManager_.entities_, 
        entityManager_.components_,
        levelProperties_,
        terrain_ 
    );
    FlushInputs();
}

void Editor::CameraUpdate() {
    camera_.Update(inputs_);
}

void Editor::MouseUpdate() {

}

void Editor::PlanarMoveUpdate() {
    if (target_ == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_];

    vec3 planarCameraForward = camera_.transform_.GetForwardVector();
    planarCameraForward.y = 0.0f;
    planarCameraForward = normalize(planarCameraForward);

    vec3 planarCameraRight = camera_.transform_.GetRightVector();
    planarCameraRight .y = 0.0f;
    planarCameraRight = normalize(planarCameraRight);

    transform.position += 
        planarCameraForward * -(float)platform_.deltaMouseY_ * 0.5f +
        planarCameraRight * (float)platform_.deltaMouseX_ * 0.5f;
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
