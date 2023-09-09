#include "SkeletonSystem.h"
#include "../Time.h"
#include "../../Constants/TimeConstants.h"

constexpr EntityKey key = GetEntityKey<StaticModelComponent, TransformComponent>();

void SkeletonSystem::CalculatePoses(
    std::array<Entity, MAX_ENTITIES>& entities,
    StaticModelComponent& modelComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;
        if (modelComponent.skeleton[i] == nullptr)
            continue;
        modelComponent.time[i] += TIMESTEP;

        const int framerate = modelComponent.skeleton[i]->GetFramerate(0);
        modelComponent.prevPose[i] = modelComponent.pose[i];
        modelComponent.skeleton[i]->GetPose(
            modelComponent.pose[i],
            1,
            modelComponent.time[i],
            transformComponent.transform[i],
            transformComponent.transformLastUpdate[i]
        );

        if (framerate == 0)
            continue;

        if (modelComponent.time[i] >= modelComponent.nextPoseUpdate[i]) {
            modelComponent.renderPose[i] = modelComponent.pose[i];
            modelComponent.nextPoseUpdate[i] += 1.0f / modelComponent.skeleton[i]->GetFramerate(0);
        }
    }
}

void SkeletonSystem::InterpPoses(
    std::array<Entity, MAX_ENTITIES>& entities,
    StaticModelComponent& modelComponent,
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
        if (modelComponent.skeleton[i] == nullptr)
            continue;

        const int framerate = modelComponent.skeleton[i]->GetFramerate(0);
        if (framerate != 0)
            continue;

        for (int p = 0; p < modelComponent.renderPose[i].size(); p++) 
            modelComponent.renderPose[i][p] = Transform::Lerp(modelComponent.prevPose[i][p], modelComponent.pose[i][p], interpAmount);
    }
}

