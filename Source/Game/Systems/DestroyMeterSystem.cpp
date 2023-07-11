#include "DestroyMeterSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityKey.h"
#include "../World/SeedManager.h"
#include "../Components/MeterComponent.h"
#include "../Components/TransformComponent.h"

constexpr EntityKey key = GetEntityKey<MeterComponent, TransformComponent>();

void DestroyMeterSystem::Execute(
    Entity* entities,
    EntityManager& entityManager,
    SeedManager& seedManager,
    MeterComponent& meterComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

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
