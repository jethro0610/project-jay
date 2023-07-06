#include "GroundStickSystem.h"
#include <glm/vec3.hpp>
#include <glm/gtx/compatibility.hpp>
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
        if (!groundTraceComponent.stickToGround[i])
            continue;
        if (!groundTraceComponent.onGround[i])
            continue;
        bool hasVelocity = entity.MatchesKey(velocityKey);

        if (!hasVelocity) {
            transformComponent.transform[i].position.y = groundTraceComponent.groundPosition[i];
        }
        else {
            vec3& velocity = velocityComponent.velocity[i];
            if (groundTraceComponent.slideOnSlopes[i]) {
                vec3 planarNormal = groundTraceComponent.groundNormal[i];
                planarNormal.y = 0.0f;
                velocity += planarNormal * -velocity.y * 0.4f;
                
                vec3 planarVelocity = vec3(velocity.x, 0.0f, velocity.z);
                if (length(planarVelocity) > 0.15f)
                    planarVelocity *= 0.99f;
                else
                    planarVelocity = vec3(0.0f);

                velocity.x = planarVelocity.x;
                velocity.z = planarVelocity.z;

                quat addRot = quat(cross(Transform::worldUp, planarVelocity * 0.015f));
                transformComponent.transform[i].rotation = addRot * transformComponent.transform[i].rotation;
            }

            float offsetGroundHeight = groundTraceComponent.groundPosition[i] + groundTraceComponent.stickOffset[i];
            float distanceToGround = offsetGroundHeight - transformComponent.transform[i].position.y;  
            velocity.y = distanceToGround / TIMESTEP;
        }
    }
}
