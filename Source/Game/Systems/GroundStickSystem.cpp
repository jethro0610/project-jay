#include "GroundStickSystem.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
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
    EntityList& entities, 
    ComponentList& components,
    World& world
) {
    auto& groundTraceComponent = std::get<GroundTraceComponent&>(components);
    auto& transformComponent = std::get<TransformComponent&>(components);
    auto& velocityComponent = std::get<VelocityComponent&>(components);

    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;
        if (!groundTraceComponent.stick[i]) continue;
        if (!groundTraceComponent.onGround[i]) continue;
        bool hasVelocity = entities[i].MatchesKey(velocityKey);

        if (!hasVelocity) {
            transformComponent.transform[i].position.y = groundTraceComponent.groundPosition[i];
        }
        else {
            vec3& velocity = velocityComponent.velocity[i];
            float offsetGroundHeight = groundTraceComponent.groundPosition[i] + groundTraceComponent.stickOffset[i];
            float distanceToGround = offsetGroundHeight - transformComponent.transform[i].position.y;  
            float stickVelocity = distanceToGround / TIMESTEP;
            if (velocity.y <= stickVelocity + 5.0f) {
                velocity.y = stickVelocity;

                if (groundTraceComponent.zeroVelocity[i]) {
                    velocity.x = 0.0f;
                    velocity.z = 0.0f;
                    velocityComponent.angularVelocity[i] = quat(1.0f, 0.0f, 0.0f, 0.0f);
                }
            }
        }
    }
}
