#include "SpreadActivatorSystem.h"
#include "Components/GroundTraceComponent.h"
#include "Components/MeterComponent.h"
#include "Components/SpreadActivatorComponent.h"
#include "Components/SpreadDetectComponent.h"
#include "Components/TransformComponent.h"
#include "Spread/SpreadManager.h"
#include "Terrain/Terrain.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<SpreadActivatorComponent, TransformComponent>();
constexpr EntityKey detectKey = GetEntityKey<SpreadDetectComponent>();
constexpr EntityKey groundKey = GetEntityKey<GroundTraceComponent>();
constexpr EntityKey meterKey = GetEntityKey<MeterComponent>();

void SpreadActivatorSystem::Execute(
    EntityList& entities, 
    ComponentList& components,
    SpreadManager& spreadManager,
    Terrain& terrain
) {
    auto& groundTraceComponent = components.Get<GroundTraceComponent>();
    auto& meterComponent = components.Get<MeterComponent>();
    auto& spreadActivatorComponent = components.Get<SpreadActivatorComponent>();
    auto& spreadDetectComponent = components.Get<SpreadDetectComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if(!entities[i].ShouldUpdate(key)) continue;

        bool onGround = false;
        if (entities[i].MatchesKey(groundKey))
            onGround = groundTraceComponent.onGround[i];
        if (!onGround && spreadActivatorComponent.groundOnly[i])
            continue;

        const int radius = spreadActivatorComponent.radius[i]; 
        if (radius == 0)
            continue;

        int* meter = nullptr;
        if (entities[i].MatchesKey(meterKey))
            meter = &meterComponent.meter[i];

        // TODO : Is the has detect here necessary? Can maybe just assign it
        // by default
        const int& amount = spreadActivatorComponent.amount[i];
        const vec3 position = transformComponent.transform[i].position;
        const bool hasDetect = entities[i].MatchesKey(detectKey);
        
        if (radius > 0) {
            const AddSpreadInfo addSpreadInfo = spreadManager.AddSpread(
                position, 
                radius, 
                std::min(amount, meter == nullptr ? INT_MAX : *meter)
            );
            if (addSpreadInfo.count == 0)
                continue;
            if (meter != nullptr && *meter > 0)
                *meter -= addSpreadInfo.count;

            if (hasDetect)
                spreadDetectComponent.lastAdd[i] = addSpreadInfo.key;
        }
        else if (radius < 0) {
            spreadManager.RemoveSpread(position, -radius, NULL_ENTITY, vec3(0.0f, (rand() % 500) * 0.01f, 0.0f));
        }
    }
}