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
    target_ = NULL_ENTITY;
    mode_ = EM_Mouse;
}

void Editor::StartEditing() {
    active_ = true;
    target_ = NULL_ENTITY;
    SetMode(EM_Mouse);
    camera_.target_ = NULL_ENTITY;
    ScreenText::SetEnabled(false);
    levelLoader_.LoadLevel(levelLoader_.DBG_currentLevel);
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
    if (mode == EM_Mouse)
        platform_.SetMouseVisible(true);
    else
        platform_.SetMouseVisible(false);
}

void Editor::Update() {
    if (platform_.pressedKeys_[GLFW_KEY_TAB])
        SetMode(EM_Mouse);
    if (platform_.pressedKeys_['Q'])
        SetMode(EM_Camera);

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

        default:
            break;
    }

    TransformSystem::ForceRenderTransforms(
        entityManager_.entities_,
        entityManager_.components_
    );
    entityManager_.SpawnEntities();
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
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    if (platform_.pressedKeys_[LEFT_MOUSE_KEY]) {
        target_ = NULL_ENTITY;
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
                target_ =  i;
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
