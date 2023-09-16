#pragma once
#include "../Entity/EntityLimits.h"
#include "../Entity/Entity.h"
#include "../Components/SkeletonComponent.h"
#include "../Components/TransformComponent.h"

namespace SkeletonSystem {
    void CalculatePoses(
        std::array<Entity, MAX_ENTITIES>& entities,
        SkeletonComponent& skeletonComponent,
        TransformComponent& transformComponent
    );

    void InterpPoses(
        std::array<Entity, MAX_ENTITIES>& entities,
        SkeletonComponent& skeletonComponent,
        TransformComponent& transformComponent,
        float interpTime
    );
}
