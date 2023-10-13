#include "DestroyMeterSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityKey.h"
#include "../World/SeedManager.h"
#include "../Components/MeterComponent.h"
#include "../Components/TransformComponent.h"

constexpr EntityKey key = GetEntityKey<MeterComponent, TransformComponent>();

void DestroyMeterSystem::Execute(
    EntityList& entities,
    ComponentList& components,
    EntityManager& entityManager,
    SeedManager& seedManager
) {
    auto& meterComponent = components.Get<MeterComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;
        if (meterComponent.destroyOnNone[i] && meterComponent.meter[i] <= 0) {
            entityManager.DestroyEntity(i);
            seedManager.CreateMultipleSeed(
                transformComponent.transform[i].position,
                meterComponent.numDestroySeeds[i],
                15
            );
        }
    }
}
