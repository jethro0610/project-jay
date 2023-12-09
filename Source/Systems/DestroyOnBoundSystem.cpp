#include "DestroyOnBoundSystem.h"
#include "Components/TransformComponent.h"

constexpr EntityKey key = GetEntityKey<TransformComponent>();

void DestroyOnBoundSystem::Execute(
    EntityList& entityList,
    ComponentList& componentList,
    DestroyList& destroyList
) {
    TransformComponent& transformComponent = componentList.Get<TransformComponent>();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityList[i];
        if (!entity.ShouldUpdate(key)) continue;
        if (!transformComponent.destroyOnBound[i]) continue;

        if (transformComponent.transform[i].position.y < Y_BOUND)
            destroyList.push_back({i, false});
    }
}
