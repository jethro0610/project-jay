#include "TransformSystem.h"

void TransformSystem::UpdateLastTransforms(EntityManager& entityManager, TransformComponent& transformComponent) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<TransformComponent>())
            continue;

        transformComponent.transformLastUpdate[i] = transformComponent.transform[i];
    }
}

void TransformSystem::UpdateRenderTransforms(float interpTime, EntityManager& entityManager, TransformComponent& transformComponent) {
    float interpAmount = interpTime / TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<TransformComponent>())
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
