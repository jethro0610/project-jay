#include "GroundStickSystem.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Game/Components/GroundTraceComponent.h"
#include "Game/Components/TransformComponent.h"
#include "Game/Components/VelocityComponent.h"
#include "Game/Terrain/Terrain.h"
#include "Game/Time/Time.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<GroundTraceComponent, TransformComponent>();
constexpr EntityKey velocityKey = GetEntityKey<VelocityComponent>();

void GroundStickSystem::Stick(
    EntityList& entities, 
    ComponentList& components,
    Terrain& world
) {
    auto& groundTraceComponent = components.Get<GroundTraceComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();

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
            float stickVelocity = distanceToGround / GlobalTime::TIMESTEP;
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
