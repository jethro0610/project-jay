#include "TransformSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/TransformComponent.h"
#include "../../Constants/TimeConstants.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<TransformComponent>();

void TransformSystem::UpdateLastTransforms(
    std::array<Entity, MAX_ENTITIES>& entities,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        transformComponent.transformLastUpdate[i] = transformComponent.transform[i];
    }
}

void TransformSystem::UpdateRenderTransforms(
    std::array<Entity, MAX_ENTITIES>& entities,
    TransformComponent& transformComponent,
    float interpTime
) {
    float interpAmount = interpTime / TIMESTEP;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        vec3 right = transformComponent.transform[i].GetRightVector(); 
        vec3 finalUp = mix(transformComponent.up[i], right, transformComponent.tilt[i]);
        finalUp = normalize(finalUp);
        quat deltaRot = rotation(Transform::worldUp, finalUp);

        if (transformComponent.interpolate[i]) {
            transformComponent.renderTransform[i] = Transform::Lerp(
                transformComponent.transformLastUpdate[i],
                transformComponent.transform[i],
                interpAmount
            );
        }
        else 
            transformComponent.renderTransform[i] = transformComponent.transform[i];

        transformComponent.renderTransform[i].rotation = deltaRot * transformComponent.renderTransform[i].rotation;
    }
}
