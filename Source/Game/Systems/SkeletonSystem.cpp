#include "SkeletonSystem.h"
#include "../Time.h"
#include "../../Constants/TimeConstants.h"

constexpr EntityKey key = GetEntityKey<SkeletonComponent, TransformComponent>();

void SkeletonSystem::CalculatePoses(
    std::array<Entity, MAX_ENTITIES>& entities,
    SkeletonComponent& skeletonComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;
        skeletonComponent.time[i] += TIMESTEP;

        int animationIndex = skeletonComponent.stateBindings[i][skeletonComponent.animState[i]];

        const int framerate = skeletonComponent.skeleton[i]->GetFramerate(animationIndex);
        skeletonComponent.prevPose[i] = skeletonComponent.pose[i];
        skeletonComponent.skeleton[i]->GetPose(
            skeletonComponent.pose[i],
            animationIndex,
            skeletonComponent.time[i],
            transformComponent.transform[i],
            transformComponent.transformLastUpdate[i]
        );

        if (framerate == 0)
            continue;

        if (skeletonComponent.time[i] >= skeletonComponent.nextPoseUpdate[i]) {
            skeletonComponent.renderPose[i] = skeletonComponent.pose[i];
            skeletonComponent.nextPoseUpdate[i] += 1.0f / skeletonComponent.skeleton[i]->GetFramerate(0);
        }
    }
}

void SkeletonSystem::InterpPoses(
    std::array<Entity, MAX_ENTITIES>& entities,
    SkeletonComponent& skeletonComponent,
    TransformComponent& transformComponent,
    float interpTime
) {
    const float interpAmount = interpTime / TIMESTEP;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        const int framerate = skeletonComponent.skeleton[i]->GetFramerate(0);
        if (framerate != 0)
            continue;

        for (int p = 0; p < skeletonComponent.renderPose[i].size(); p++) 
            skeletonComponent.renderPose[i][p] = Transform::Lerp(skeletonComponent.prevPose[i][p], skeletonComponent.pose[i][p], interpAmount);
    }
}

