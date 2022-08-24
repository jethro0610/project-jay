#pragma once
#include "../Entity/Entity.h"
#include "../Components/TransformComponent.h"
#include "../TimeConstants.h"

namespace TransformSystem {
    void UpdateLastTransforms(Entity* entities, TransformComponent& transformComponent);
    void UpdateRenderTransforms(float interpTime, Entity* entities, TransformComponent& transformComponent);
}