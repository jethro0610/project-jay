#include "SpreadDetectSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/SpreadManager.h"
#include "../Components/SpreadDetectComponent.h"
#include "../Components/TransformComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<SpreadDetectComponent, TransformComponent>();

void SpreadDetectSystem::Execute(
    std::array<Entity, MAX_ENTITIES>& entities,
    SpreadManager& spreadManager,
    SpreadDetectComponent& spreadDetectComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        bool& detected = spreadDetectComponent.deteced[i];
        detected = false;

        SpreadKey currentKey = spreadManager.GetKey(transformComponent.transform[i].position);
        if (!spreadManager.SpreadIsActive(currentKey)) {
            spreadDetectComponent.lastAdd[i] = NO_ADD;
            continue;
        }

        if (currentKey == spreadDetectComponent.lastKey[i])
            continue;
        spreadDetectComponent.lastKey[i] = currentKey;

        if (
            spreadDetectComponent.lastAdd[i] != NO_ADD && 
            distance(vec2(spreadDetectComponent.lastAdd[i]), vec2(currentKey)) <= MAX_ADD_DISTANCE
        ) continue;

        detected = true;
        spreadDetectComponent.lastAdd[i] = NO_ADD;  
        // Maybe have the spread wiggle on entry?
    }
}
