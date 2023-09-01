#include "SkeletonSystem.h"
#include "../Time.h"

constexpr EntityKey key = GetEntityKey<StaticModelComponent, TransformComponent>();

void SkeletonSystem::CalculatePoses(
    std::array<Entity, MAX_ENTITIES>& entities,
    StaticModelComponent& modelComponent,
    TransformComponent& transformComponent,
    float deltaTime
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;
        if (modelComponent.skeleton[i] == nullptr)
            continue;

        modelComponent.time[i] += GlobalTime::GetDeltaTime();
        modelComponent.skeleton[i]->GetPose(
            modelComponent.pose[i],
            0,
            modelComponent.time[i],
            transformComponent.renderTransform[i],
            transformComponent.transformLastPose[i]
        );
        transformComponent.transformLastPose = transformComponent.renderTransform;

        if (modelComponent.time[i] >= modelComponent.nextPoseUpdate[i]) {
            modelComponent.renderPose[i] = modelComponent.pose[i];
            modelComponent.nextPoseUpdate[i] += 1/6.0f;
        }
    }
}
