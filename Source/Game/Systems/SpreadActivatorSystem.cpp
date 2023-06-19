#include <math.h>
#include "SpreadActivatorSystem.h"
#include "../Entity/Entity.h"
#include "../../Helpers/EntityHelpers.h"
#include "../World/SpreadManager.h"
#include "../World/World.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/MeterComponent.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Components/SpreadDetectComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<SpreadActivatorComponent, TransformComponent>();
constexpr EntityKey detectKey = GetEntityKey<SpreadDetectComponent>();
constexpr EntityKey groundKey = GetEntityKey<GroundTraceComponent>();
constexpr EntityKey meterKey = GetEntityKey<MeterComponent>();

void SpreadActivatorSystem::Execute(
    Entity* entities, 
    SpreadManager& spreadManager,
    World& world,
    GroundTraceComponent& groundTraceComponent,
    MeterComponent& meterComponent,
    SpreadActivatorComponent& spreadActivatorComponent,
    SpreadDetectComponent& spreadDetectComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        bool onGround = false;
        if (entity.MatchesKey(groundKey))
            onGround = groundTraceComponent.onGround[i];
        if (!onGround && spreadActivatorComponent.groundOnly[i])
            continue;

        const int16_t radius = spreadActivatorComponent.radius[i]; 
        if (radius == 0)
            continue;

        uint16_t* meter = nullptr;
        if (entity.MatchesKey(meterKey))
            meter = &meterComponent.meter[i];

        // TODO : Is the has detect here necessary? Can maybe just assign it
        // by default
        const uint16_t& amount = spreadActivatorComponent.amount[i];
        const vec3 position = transformComponent.transform[i].position_;
        const bool hasDetect = entity.MatchesKey(detectKey);
        
        if (radius > 0) {
            const AddSpreadInfo addSpreadInfo = spreadManager.AddSpread(
                position, 
                radius, 
                std::min(amount, meter == nullptr ? UINT16_MAX : *meter)
            );
            if (addSpreadInfo.count == 0)
                continue;
            if (meter != nullptr && *meter > 0)
                *meter -= addSpreadInfo.count;

            if (hasDetect)
                spreadDetectComponent.lastAdd[i] = addSpreadInfo.key;
        }
        else if (radius < 0) {
            spreadManager.RemoveSpread(position, -radius, i, vec3(0.0f, (rand() % 1000) * 0.01f, 0.0f));
        }
    }
}
