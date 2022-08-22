#include "TransformSystem.h"

void TransformSystem::UpdateLastTransforms(ActiveEntityComponents& activeEntityComponents, TransformComponents& transformComponents) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!activeEntityComponents.active[i])
            continue;

        transformComponents.transformLastUpdate[i] = transformComponents.transform[i];
    }
}
void TransformSystem::UpdateRenderTransforms(float interpTime, ActiveEntityComponents& activeEntityComponents, TransformComponents& transformComponents) {
    float interpAmount = interpTime / TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!activeEntityComponents.active[i])
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