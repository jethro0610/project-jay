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
        if (groundTraceComponent.stickType[i] < StickType::StickOnly)
            continue;
        if (!groundTraceComponent.onGround[i])
            continue;
        bool hasVelocity = entity.MatchesKey(velocityKey);

        if (!hasVelocity) {
            transformComponent.transform[i].position.y = groundTraceComponent.groundPosition[i];
        }
        else {
            vec3& velocity = velocityComponent.velocity[i];
            float distanceToGround = groundTraceComponent.groundPosition[i] - transformComponent.transform[i].position.y;  
            velocity.y = distanceToGround / TIMESTEP;
        }
    }
}
