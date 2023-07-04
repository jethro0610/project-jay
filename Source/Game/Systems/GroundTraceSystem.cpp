#include "GroundTraceSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/World.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Logging/Logger.h"
#include <glm/gtx/projection.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

constexpr EntityKey key = GetEntityKey<GroundTraceComponent, TransformComponent>();
constexpr EntityKey velocityKey = GetEntityKey<VelocityComponent>();

void GroundTraceSystem::Execute(
    Entity* entities,
    World& world,
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;
        bool hasVelocity = entity.MatchesKey(velocityKey);

        float traceDistance = groundTraceComponent.distance[i];
        groundTraceComponent.onGroundLastFrame[i] = groundTraceComponent.onGround[i];

        // Zero out the y velocity and trace far down if on ground
        if (groundTraceComponent.onGroundLastFrame[i])
            traceDistance = 100.0f;

        vec3 position = transformComponent.transform[i].position;
        vec2 worldDistance = world.GetDistance(position);
        groundTraceComponent.groundPosition[i] = worldDistance.y;
        groundTraceComponent.groundNormal[i] = world.GetNormal(position);
        float distanceToSurface = position.y - worldDistance.y;

        if (distanceToSurface < traceDistance && worldDistance.x > -32.0f)
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
    }
}
