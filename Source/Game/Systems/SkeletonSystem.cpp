#include "SkeletonSystem.h"
#include "../Entity/Entity.h"
#include "../Components/SkeletonComponent.h"
#include "../Components/TransformComponent.h"
#include "../Time.h"
#include "../../Constants/TimeConstants.h"

constexpr EntityKey key = GetEntityKey<SkeletonComponent, TransformComponent>();

void SkeletonSystem::CalculatePoses(
    EntityList& entities,
    ComponentList& components
) {
    auto& skeletonComponent = components.Get<SkeletonComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;

        skeletonComponent.time[i] += TIMESTEP;
        skeletonComponent.prevTime[i] += TIMESTEP;

        int& prevAnimationIndex = skeletonComponent.prevAnimationIndex[i];
        int& nextAnimationIndex = skeletonComponent.nextAnimationIndex[i];
        int& animationIndex = skeletonComponent.animationIndex[i];
        float& prevTime = skeletonComponent.prevTime[i];
        float& time = skeletonComponent.time[i];
        float& transitionLength = skeletonComponent.transitionLength[i];
        float& transitionTime = skeletonComponent.transitionTime[i];
        int prevFramerate = skeletonComponent.skeleton[i]->GetFramerate(prevAnimationIndex);
        int curFramerate = skeletonComponent.skeleton[i]->GetFramerate(animationIndex);

        int framerate = transitionTime < transitionLength ? 
            std::lerp(prevFramerate, curFramerate, std::clamp(transitionTime / transitionLength, 0.0f, 1.0f)) :
            curFramerate;

        if (animationIndex != nextAnimationIndex) {
            prevAnimationIndex = animationIndex;
            animationIndex = nextAnimationIndex; 
            prevTime = time;
            time = 0.0f;

            transitionLength = 0.35f;
            transitionTime = 0.0f;
            if (nextAnimationIndex == 4)
                transitionTime = transitionLength;
        }

        skeletonComponent.prevPose[i] = skeletonComponent.pose[i];

        if (transitionTime >= transitionLength || transitionLength == 0.0f) {
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
        
        int frame = floor(skeletonComponent.time[i] * 60.0f);
        int framesTillUpate = 60 / framerate;

        if (frame % framesTillUpate == 0)
            skeletonComponent.renderPose[i] = skeletonComponent.pose[i];

        skeletonComponent.transitionThisTick[i] = false;
    }
}

void SkeletonSystem::InterpPoses(
    EntityList& entities,
    ComponentList& components,
    float interpTime
) {
    auto& skeletonComponent = components.Get<SkeletonComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    const float interpAmount = interpTime / TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;

        const int framerate = skeletonComponent.skeleton[i]->GetFramerate(skeletonComponent.animationIndex[i]);
        if (framerate != 0)
            continue;

        for (int p = 0; p < skeletonComponent.renderPose[i].size(); p++) 
            skeletonComponent.renderPose[i][p] = Transform::Lerp(
                skeletonComponent.prevPose[i][p], 
                skeletonComponent.pose[i][p], 
                interpAmount
            );
    }
}

