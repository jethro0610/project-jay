#include "TransformSystem.h"
#include "../Entity/EntityManager.h"
#include "../../Constants/TimeConstants.h"

void TransformSystem::UpdateLastTransforms(EntityManager& entityManager) {
    TransformComponent& transformComponent = entityManager.transformComponent_;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent(transformComponent))
            continue;

        transformComponent.transformLastUpdate[i] = transformComponent.transform[i];
    }
}

void TransformSystem::UpdateRenderTransforms(EntityManager& entityManager, float interpTime) {
    TransformComponent& transformComponent = entityManager.transformComponent_;
    float interpAmount = interpTime / TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent(transformComponent))
            continue;

        if (transformComponent.interpolate[i]) {
            transformComponent.renderTransform[i] = Transform::Lerp(
                transformComponent.transformLastUpdate[i],
                transformComponent.transform[i],
                interpAmount
            );
        }
        else 
            transformComponent.renderTransform[i] = transformComponent.transform[i];
    }
}
