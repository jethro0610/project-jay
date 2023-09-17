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
        skeletonComponent.prevTime[i] += TIMESTEP;

        int& prevAnimationIndex = skeletonComponent.prevAnimationIndex[i];
        int& nextAnimationIndex = skeletonComponent.nextAnimationIndex[i];
        int& animationIndex = skeletonComponent.animationIndex[i];
        float& prevTime = skeletonComponent.prevTime[i];
        float& time = skeletonComponent.time[i];
        float& transitionLength = skeletonComponent.transitionLength[i];
        float& transitionTime = skeletonComponent.transitionTime[i];
        float & nextPoseUpdate = skeletonComponent.nextPoseUpdate[i];
        int framerate = skeletonComponent.skeleton[i]->GetFramerate(animationIndex);

        if (animationIndex != nextAnimationIndex) {
            prevAnimationIndex = animationIndex;
            animationIndex = nextAnimationIndex; 
            prevTime = time;

            transitionLength = 0.35f;
            transitionTime = 0.0f;
        }

        skeletonComponent.prevPose[i] = skeletonComponent.pose[i];

        if (skeletonComponent.transitionTime > skeletonComponent.transitionLength) {
            skeletonComponent.skeleton[i]->GetPose(
                skeletonComponent.pose[i],
                animationIndex,
                time,
                transformComponent.transform[i],
                transformComponent.transformLastUpdate[i]
            );
            // prevAnimationIndex = animationIndex;
        }
        else {
            skeletonComponent.skeleton[i]->GetBlendedPose(
                skeletonComponent.pose[i],
                prevAnimationIndex,
                animationIndex,
                prevTime,
                time,
                transitionTime / transitionLength,
                transformComponent.transform[i],
                transformComponent.transformLastUpdate[i]
            );
            transitionTime += TIMESTEP;
        }

        if (framerate == 0)
            continue;

        if (skeletonComponent.time[i] >= skeletonComponent.nextPoseUpdate[i]) {
            skeletonComponent.renderPose[i] = skeletonComponent.pose[i];
            skeletonComponent.nextPoseUpdate[i] += 1.0f / framerate;
        }

        skeletonComponent.transitionThisTick[i] = false;
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

