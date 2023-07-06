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
    Entity* entities,
    World& world,
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
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

        if (
            !groundTraceComponent.forceNoGroundThisFrame[i]  &&
            distanceToSurface < traceDistance
        )
            groundTraceComponent.onGround[i] = true;
        else
            groundTraceComponent.onGround[i] = false;

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

        groundTraceComponent.forceNoGroundThisFrame[i] = false;
    }
}
