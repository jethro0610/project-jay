#include "TransformSystem.h"
#include "Components/TransformComponent.h"
#include "Time/Time.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<TransformComponent>();

void TransformSystem::UpdateLastTransforms(
    EntityList& entities,
    ComponentList& components 
) {
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;

        transformComponent.transformLastUpdate[i] = transformComponent.transform[i];
        if (!transformComponent.useTilt[i])
            continue;

        transformComponent.lastTilt[i] = transformComponent.tilt[i];
        transformComponent.lastUp[i] = transformComponent.up[i];
    }
}

void TransformSystem::UpdateRenderTransforms(
    EntityList& entities,
    ComponentList& components,
    float interpTime
) {
    auto& transformComponent = components.Get<TransformComponent>();
    float interpAmount = interpTime / GlobalTime::TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
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

        if (!transformComponent.useTilt[i])
            continue;

        vec3 renderUp = mix(transformComponent.lastUp[i], transformComponent.up[i], interpAmount);
        float renderTilt = std::lerp(transformComponent.lastTilt[i], transformComponent.tilt[i], interpAmount);
        vec3 right = transformComponent.transform[i].GetRightVector(); 
        vec3 finalUp = mix(transformComponent.up[i], right, transformComponent.tilt[i]);
        finalUp = normalize(finalUp);
        quat deltaRot = rotation(Transform::worldUp, finalUp);
        transformComponent.renderTransform[i].rotation = deltaRot * transformComponent.renderTransform[i].rotation;
    }
}
