#include "GroundTraceSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/World.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../../Logging/Logger.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<GroundTraceComponent, TransformComponent>();

void GroundTraceSystem::Execute(
    std::array<Entity, MAX_ENTITIES>& entities,
    World& world,
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        float traceDistance = groundTraceComponent.distance[i] + groundTraceComponent.stickOffset[i];
        groundTraceComponent.onGroundLastFrame[i] = groundTraceComponent.onGround[i];

        vec3 position = transformComponent.transform[i].position;
        float groundHeight = world.GetHeight(position);
        groundTraceComponent.groundPosition[i] = groundHeight;
        groundTraceComponent.groundNormal[i] = world.GetNormal(position);
        float distanceToSurface = position.y - groundHeight;

        if (distanceToSurface < traceDistance) {
            groundTraceComponent.onGround[i] = true;
            if (groundTraceComponent.alignStrength[i] != 0.0f)
                transformComponent.up[i] = mix(
                    Transform::worldUp, 
                    groundTraceComponent.groundNormal[i], 
                    groundTraceComponent.alignStrength[i]
                );
        }
        else {
            groundTraceComponent.onGround[i] = false;
            transformComponent.up[i] = Transform::worldUp;
        }

        // Check if the entity entered/exited the ground on this frame
        if (!groundTraceComponent.onGroundLastFrame[i] && groundTraceComponent.onGround[i])
            groundTraceComponent.enteredGround[i] = true;
        else
            groundTraceComponent.enteredGround[i] = false;

        if (groundTraceComponent.onGroundLastFrame[i] && !groundTraceComponent.onGround[i]) {
            groundTraceComponent.exitedGround[i] = true;
        }
        else
            groundTraceComponent.exitedGround[i] = false;
    }
}
