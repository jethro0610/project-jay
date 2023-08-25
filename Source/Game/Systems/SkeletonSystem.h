#pragma once
#include "../Entity/EntityLimits.h"
#include "../Entity/Entity.h"
#include "../Components/StaticModelComponent.h"
#include "../Components/TransformComponent.h"

namespace SkeletonSystem {
    void CalculatePoses(
        std::array<Entity, MAX_ENTITIES>& entities,
        StaticModelComponent& modelComponent,
        TransformComponent& transformComponent,
        float deltaTime
    );
}
