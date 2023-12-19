#include "TerrainTranslateMode.h"
#include <glm/vec3.hpp>
using namespace glm;

TerrainTranslateMode::TerrainTranslateMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

void TerrainTranslateMode::Update() {
    if (target_.Get() == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];

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
