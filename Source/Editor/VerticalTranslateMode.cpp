#include "VerticalTranslateMode.h"
using namespace glm;

VerticalTranslateMode::VerticalTranslateMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

void VerticalTranslateMode::Update() {
    if (target_.Get() == NULL_ENTITY) return;

    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    Transform& transform = transformComponent.transform[target_.Get()];
    transform.position.y += -(float)platform_.deltaMouseY_ * 0.1f;
}
