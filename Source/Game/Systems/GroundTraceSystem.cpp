#include "GroundTraceSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/World.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
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
        bool isRising = hasVelocity && velocityComponent.velocity[i].y > 0.0f;

        float traceDistance = groundTraceComponent.distance[i];
        groundTraceComponent.onGroundLastFrame[i] = groundTraceComponent.onGround[i];

        // Zero out the y velocity and trace far down if on ground
        if (groundTraceComponent.onGroundLastFrame[i] && !isRising) {
            traceDistance = 100.0f;
            if (hasVelocity)
                velocityComponent.velocity[i].y = 0.0f;
        }
        else if (isRising) // If its rising then don't trace
            traceDistance = 0.0f;

        // Raymarch towards the ground
        vec3 position = transformComponent.transform[i].position_;
        float worldHeight = world.GetHeight(position);
        float distanceToSurface = position.y - worldHeight;

        // Hit solving
        if (distanceToSurface < traceDistance) {
            groundTraceComponent.onGround[i] = true;
            vec3 groundNormal = world.GetNormal(position);
            groundTraceComponent.groundPosition[i] = worldHeight;
            groundTraceComponent.groundNormal[i] = groundNormal;
        }
        else {
            groundTraceComponent.onGround[i] = false;
        }

        // Check if the entity entered/exited the ground on this frame
        if (!groundTraceComponent.onGroundLastFrame[i] && groundTraceComponent.onGround[i])
            groundTraceComponent.enteredGround[i] = true;
        else
            groundTraceComponent.enteredGround[i] = false;

        if (groundTraceComponent.onGroundLastFrame[i] && !groundTraceComponent.onGround[i])
            groundTraceComponent.exitedGround[i] = true;
        else
            groundTraceComponent.exitedGround[i] = false;
    }
}
