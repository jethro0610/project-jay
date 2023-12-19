#include "UniformScaleMode.h"
using namespace glm;

UniformScaleMode::UniformScaleMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

void UniformScaleMode::Update() {
    if (target_.Get() == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];
    transform.scale.x = max(transform.scale.x -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
    transform.scale.y = max(transform.scale.y -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
    transform.scale.z = max(transform.scale.z -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
}
