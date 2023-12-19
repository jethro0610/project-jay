#include "VerticalScaleMode.h"
using namespace glm;

VerticalScaleMode::VerticalScaleMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = false;
}

void VerticalScaleMode::Update() {
    if (target_.Get() == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];
    transform.scale.y = max(transform.scale.y -(float)platform_.deltaMouseY_ * 0.1f, 0.05f);
}
