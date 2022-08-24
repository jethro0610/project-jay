#include "TransformSystem.h"

void TransformSystem::UpdateLastTransforms(Entity* entities, TransformComponents& transformComponents) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.HasComponent<TransformComponents>())
            continue;

        transformComponents.transformLastUpdate[i] = transformComponents.transform[i];
    }
}

void TransformSystem::UpdateRenderTransforms(float interpTime, Entity* entities, TransformComponents& transformComponents) {
    float interpAmount = interpTime / TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.HasComponent<TransformComponents>())
            continue;

        if (transformComponents.interpolate[i]) {
            transformComponents.renderTransform[i] = Transform::Lerp(
                transformComponents.transformLastUpdate[i],
                transformComponents.transform[i],
                interpAmount
            );
        }
        else 
            transformComponents.renderTransform[i] = transformComponents.transform[i];
    }
}