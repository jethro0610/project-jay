#include "TransformSystem.h"

void TransformSystem::UpdateLastTransforms(Entity* entities, TransformComponents& transformComponents) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].componentMask.test(TransformComponents::ID))
            continue;

        transformComponents.transformLastUpdate[i] = transformComponents.transform[i];
    }
}

void TransformSystem::UpdateRenderTransforms(float interpTime, Entity* entities, TransformComponents& transformComponents) {
    float interpAmount = interpTime / TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].componentMask.test(TransformComponents::ID))
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