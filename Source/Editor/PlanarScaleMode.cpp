#include "PlanarScaleMode.h"
using namespace glm;

PlanarScaleMode::PlanarScaleMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

void PlanarScaleMode::Update() {
    if (target_.Get() == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];
    transform.scale.x = max(transform.scale.x + (float)platform_.deltaMouseX_ * 0.1f, 0.05f);
    transform.scale.z = max(transform.scale.z + (float)platform_.deltaMouseX_ * 0.1f, 0.05f);
}
