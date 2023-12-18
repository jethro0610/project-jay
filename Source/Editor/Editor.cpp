#include "Editor.h"
#include "Rendering/Renderer.h"
#include "Systems/Systems.h"
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
running_(running)
{
    active_ = false;
    SetTarget(NULL_ENTITY);
    mode_ = EM_Mouse;

    modeText_.properties_.scale = 40.0f;
    modeText_.properties_.hAlignment = Text::RIGHT_ALIGN;
    modeText_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    modeText_.properties_.hAnchor = Text::RIGHT_ALIGN;
    modeText_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    targetText_.properties_.scale = 40.0f;
    targetText_.properties_.position.y -= 40.0f;
    targetText_.properties_.hAlignment = Text::RIGHT_ALIGN;
    targetText_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    targetText_.properties_.hAnchor = Text::RIGHT_ALIGN;
    targetText_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    inputText_.properties_.scale = 40.0f;
    inputText_.properties_.hAlignment = Text::LEFT_ALIGN;
    inputText_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    inputText_.properties_.hAnchor = Text::LEFT_ALIGN;
    inputText_.properties_.vAnchor = Text::BOTTOM_ALIGN;
}

void Editor::StartEditing() {
    active_ = true;
    SetTarget(NULL_ENTITY);
    SetMode(EM_Camera);
    camera_.target_ = NULL_ENTITY;
    ScreenText::SetEnabled(false);
    levelLoader_.LoadLevel(levelLoader_.DBG_currentLevel);
}

void Editor::StopEditing() {
    active_ = false;
    SetTarget(NULL_ENTITY);
    camera_.target_ = PLAYER_ENTITY;
    platform_.SetMouseVisible(false);
    ScreenText::Clear();
    ScreenText::SetEnabled(false);
    SaveLevel();
}

void Editor::SetTarget(EntityID target) {
    if (target_ != NULL_ENTITY)
        entityManager_.entities_[target_].DBG_selected = false;

    target_ = target;
    if (target_ == NULL_ENTITY)
        targetText_ = "";
    else {
        targetText_ = entityManager_.entities_[target_].DBG_name + '(' + std::to_string(target_) + ')';
        entityManager_.entities_[target_].DBG_selected = true;
    }
}

std::string GetModeName(EditorMode mode) {
    switch(mode) {
        case EM_Camera:
            return "Free Camera";
        
        case EM_Mouse:
            return "Cursor";

        case EM_AlignMove:
            return "Terrain Aligned Translation";

        case EM_PlanarMove:
            return "Planar Translation";

        case EM_VeritcalMove:
            return "Vertical Translation";

        case EM_PlanarScale:
            return "Planar Scale";

        case EM_VerticalScale:
            return "Vertical Scale";

        case EM_Scale:
            return "Uniform Scale";

        case EM_Spawn:
            return "Spawn Entity";

        default:
            return "Unknown";
    }
}

void Editor::SetMode(EditorMode mode) {
    mode_ = mode;
    if (mode == EM_Mouse)
        platform_.SetMouseVisible(true);
    else
        platform_.SetMouseVisible(false);

    if (mode_ == EM_Camera)
        inputText_.Clear();

    modeText_ = GetModeName(mode_);
    FlushInputs();
}

void Editor::Update() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();

    if (platform_.pressedKeys_[GLFW_KEY_TAB])
        SetMode(EM_Camera);

    if (mode_ != EM_Spawn) {
        if (platform_.pressedKeys_['Q'])
            SetMode(EM_Mouse);

        if (platform_.pressedKeys_['E'] && !platform_.heldKeys_[GLFW_KEY_LEFT_CONTROL])
            SetMode(EM_PlanarMove);
        if (platform_.pressedKeys_['R'])
            SetMode(EM_VeritcalMove);
        if (platform_.pressedKeys_['T'])
            SetMode(EM_AlignMove);

        if (platform_.pressedKeys_['X'])
            SetMode(EM_PlanarScale);
        if (platform_.pressedKeys_['Z'])
            SetMode(EM_VerticalScale);
        if (platform_.pressedKeys_['C'])
            SetMode(EM_Scale);
        if (platform_.pressedKeys_['I'])
            SetMode(EM_Spawn);

        if (platform_.pressedKeys_[GLFW_KEY_DELETE] && target_ != NULL_ENTITY) {
            entityManager_.destroyList_.push_back({target_, false});
            SetTarget(NULL_ENTITY);
        }

        if (
            platform_.heldKeys_[GLFW_KEY_LEFT_CONTROL] && 
            platform_.pressedKeys_['D'] && 
            target_ != NULL_ENTITY
        ) {
            entityManager_.spawnList_.push_back({
                resourceManager_.GetEntityDescription(entityManager_.entities_[target_].DBG_name), 
                transformComponent.transform[target_]
            });
        }
    }

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

        case EM_AlignMove: {
            AlignMoveUpdate();
            break;
        }

        case EM_PlanarMove: {
            PlanarMoveUpdate();
            break;
        }

        case EM_VeritcalMove: {
            VerticalMoveUpdate();
            break;
        }

        case EM_PlanarScale: {
            PlanarScaleUpdate();
            break;
        }

        case EM_VerticalScale: {
            VerticalScaleUpdate();
            break;
        }

        case EM_Scale: {
            ScaleUpdate();
            break;
        }

        case EM_Spawn: {
            SpawnUpdate();
            break;
        }

        default:
            break;
    }

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

void Editor::CameraUpdate() {
    camera_.Update(inputs_);
}

void Editor::MouseUpdate() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    if (platform_.pressedKeys_[LEFT_MOUSE_KEY]) {
        SetTarget(NULL_ENTITY);
        float maxDist = INFINITY;
        vec3 mouseRay = GetMouseRay();
        for (int i = 0; i < MAX_ENTITIES; i++) {
            const Entity& entity = entityManager_.entities_[i];
            if (!entity.alive_) continue;

            Transform& transform = transformComponent.transform[i];
            float dist = distance(camera_.transform_.position, transform.position);
            if (
                Ray::RayHitCollider(camera_.transform_.position, mouseRay, transform, entity.DBG_collider) &&
                dist < maxDist
            ) {
                maxDist = dist;
                SetTarget(i);
            }
        }
    }
}

void Editor::AlignMoveUpdate() {
    if (target_ == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_];

    vec3 planarCameraForward = camera_.transform_.GetForwardVector();
    planarCameraForward.y = 0.0f;
    planarCameraForward = normalize(planarCameraForward);

    vec3 planarCameraRight = camera_.transform_.GetRightVector();
    planarCameraRight.y = 0.0f;
    planarCameraRight = normalize(planarCameraRight);

    transform.position += 
        planarCameraForward * -(float)platform_.deltaMouseY_ * 0.1f +
        planarCameraRight * (float)platform_.deltaMouseX_ * 0.1f;

    transform.position.y = terrain_.GetHeight(vec2(transform.position.x, transform.position.z));
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
        planarCameraForward * -(float)platform_.deltaMouseY_ * 0.1f +
        planarCameraRight * (float)platform_.deltaMouseX_ * 0.1f;
}

void Editor::VerticalMoveUpdate() {
    if (target_ == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_];
    transform.position.y += -(float)platform_.deltaMouseY_ * 0.1f;
}

void Editor::PlanarScaleUpdate() {
    if (target_ == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_];
    transform.scale.x = max(transform.scale.x + (float)platform_.deltaMouseX_ * 0.1f, 0.05f);
    transform.scale.z = max(transform.scale.z + (float)platform_.deltaMouseX_ * 0.1f, 0.05f);
}

void Editor::VerticalScaleUpdate() {
    if (target_ == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_];
    transform.scale.y = max(transform.scale.y -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
}

void Editor::ScaleUpdate() {
    if (target_ == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_];
    transform.scale.x = max(transform.scale.x -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
    transform.scale.y = max(transform.scale.y -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
    transform.scale.z = max(transform.scale.z -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
}

void Editor::SpawnUpdate() {
    if (platform_.pressedKeys_[GLFW_KEY_BACKSPACE]) 
        inputText_.RemoveLast();

    for (int i = 0; i < NUM_KEYS; i++) {
        if (i < GLFW_KEY_SPACE || i > 'Z')
            continue;

        if (!platform_.pressedKeys_[i])
            continue;

        if (
            !platform_.heldKeys_[GLFW_KEY_LEFT_SHIFT] &&
            !platform_.heldKeys_[GLFW_KEY_RIGHT_SHIFT]
        )
            inputText_ += tolower(i);
        else if (i == '-')
            inputText_ += 95;
        else
            inputText_ += i;
    }

    if (platform_.pressedKeys_[GLFW_KEY_ENTER]) {
        const std::string entityName = "e_" + inputText_.ToString();
        inputText_.Clear();
        SetMode(EM_Mouse);

        if (!resourceManager_.HasEntityDescription(entityName)) {
            if (!std::filesystem::exists("entities/" + entityName + ".json"))
                return;

            DependencyList dList = levelLoader_.GenerateEntityDependencyList(entityName);
            resourceManager_.LoadDependencies(dList);
        }

        Transform spawnTransform;
        spawnTransform.position = camera_.transform_.position + camera_.transform_.GetForwardVector() * 20.0f;
        entityManager_.spawnList_.push_back({
            resourceManager_.GetEntityDescription(entityName),
            spawnTransform
        });
    }
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

vec3 Editor::GetMouseRay() {
    float mouseX = platform_.mouseX_ / (1280  * 0.5f) - 1.0f;
    float mouseY = platform_.mouseY_ / (720 * 0.5f) - 1.0f;

    glm::mat4 invVP = glm::inverse(renderer_.GetProjectionMatrix() * camera_.GetViewOnlyMatrix());
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}
