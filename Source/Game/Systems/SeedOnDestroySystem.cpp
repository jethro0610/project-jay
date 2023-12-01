#include "SeedOnDestroySystem.h"
#include "../Entity/Entity.h"
#include "../World/SeedManager.h"
#include "../Components/TransformComponent.h"

void SeedOnDestroySystem::Execute(
    EntityList& entityList,
    ComponentList& componentList,
    DestroyList& destroyList,
    SeedManager& seedManager
) {
    TransformComponent& transformComponent = componentList.Get<TransformComponent>();

    for (int i = 0; i < destroyList.size(); i++) {
        const EntityID entityId = destroyList[i];
        const Entity& entity = entityList[entityId];
        if (entity.seedsOnDestroy_ <= 0) continue;

        seedManager.CreateMultipleSeed(
            transformComponent.transform[entityId].position, 
            entity.seedsOnDestroy_,
            entity.seedsRadius_
        );
    }
}
