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

        if (modelComponent.snakeChainList[i].size() > 0) {
            modelComponent.skeleton[i]->GetWorldPose(
                modelComponent.worldPose[i],
                transformComponent.renderTransform[i],
                modelComponent.snakeChainList[i],
                deltaTime,
                0,
                GlobalTime::GetTime()
            );
        }
        else {
            modelComponent.skeleton[i]->GetWorldPose(
                modelComponent.worldPose[i],
                transformComponent.renderTransform[i],
                0,
                GlobalTime::GetTime()
            );
        }

    }
}
