#pragma once
#include "../Entity/EntityManager.h"
#include "../Components/TransformComponent.h"

namespace TransformSystem {
    void UpdateLastTransforms(EntityManager& entities, TransformComponent& transformComponent);
    void UpdateRenderTransforms(float interpTime, EntityManager& entities, TransformComponent& transformComponent);
}
