#include "DestroyMeterSystem.h"
#include "Game/Components/MeterComponent.h"
#include "Game/Components/TransformComponent.h"

constexpr EntityKey key = GetEntityKey<MeterComponent, TransformComponent>();

void DestroyMeterSystem::Execute(
    EntityList& entities,
    ComponentList& components,
    DestroyList& destroyList
) {
    auto& meterComponent = components.Get<MeterComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;
        if (meterComponent.destroyOnNone[i] && meterComponent.meter[i] <= 0) {
            destroyList.push_back({i, true});
        }
    }
}
