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
        const DestroyInfo destroyInfo = destroyList[i];
        const Entity& entity = entityList[destroyInfo.entityId];
        if (entity.seedsOnDestroy_ <= 0 || !destroyInfo.doDestroyEffects) continue;

        seedManager.CreateMultipleSeed(
            transformComponent.transform[destroyInfo.entityId].position, 
            entity.seedsOnDestroy_,
            entity.seedsRadius_
        );
    }
}
