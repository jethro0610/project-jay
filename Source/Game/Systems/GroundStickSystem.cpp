#include "GroundStickSystem.h"
#include <glm/vec3.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/World.h"
#include "../../Logging/Logger.h"
#include "../../Constants/TimeConstants.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<GroundTraceComponent, TransformComponent>();
void GroundStickSystem::Step(
    Entity* entities, 
    World& world, 
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StepUp)
            transformComponent.transform[i].position.y += 1.0f;
    }
}


constexpr EntityKey velocityKey = GetEntityKey<VelocityComponent>();
void GroundStickSystem::Stick(
    Entity* entities, 
    World& world, 
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;
        bool hasVelocity = entity.MatchesKey(velocityKey);

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StickOnly)
            transformComponent.transform[i].position.y = groundTraceComponent.groundPosition[i];
        else if (hasVelocity && groundTraceComponent.exitedGround[i] && velocityComponent.velocity[i].y <= 0.0f) {
            vec3& velocity = velocityComponent.velocity[i];

            // Calculate what the velocity should be if the object were to continue sticking
            vec3 deltaPosition = transformComponent.transform[i].position; 
            deltaPosition.y = groundTraceComponent.groundPosition[i];
            deltaPosition += velocity * TIMESTEP;
            vec3 trueVelocity = deltaPosition - transformComponent.transform[i].position;
            trueVelocity /= TIMESTEP;
            velocity = trueVelocity;

            transformComponent.transform[i].rotation = quatLookAtRH(normalize(vec3(velocity.x, 0.0f, velocity.z)), Transform::worldUp);
        }
    }
}
