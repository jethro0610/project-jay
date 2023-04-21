#include "SpreadDetectSystem.h"
#include "../Entity/Entity.h"
#include "../../Helpers/EntityHelpers.h"
#include "../World/SpreadManager.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Components/SpreadDetectComponent.h"
#include "../Components/TransformComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<SpreadDetectComponent, TransformComponent>();

void SpreadDetectSystem::Execute(
    Entity* entities,
    SpreadManager& spreadManager,
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

        bool& detected = spreadDetectComponent.deteced[i];
        detected = false;

        ivec2 currentKey = spreadManager.WorldPositionToSpreadKey(transformComponent.transform[i].position_);
        if (!spreadManager.SpreadIsActive(currentKey)) {
            spreadDetectComponent.lastAdd[i] = NO_ADD;
            continue;
        }

        if (currentKey == spreadDetectComponent.lastKey[i])
            continue;
        spreadDetectComponent.lastKey[i] = currentKey;

        const bool hasActivator = entity.HasComponent<SpreadActivatorComponent>();
        if (hasActivator && distance(vec2(spreadDetectComponent.lastAdd[i]), vec2(currentKey)) <= MAX_ADD_DISTANCE)
            continue;

        detected = true;
        spreadDetectComponent.lastAdd[i] = NO_ADD;  
        // Maybe have the spread wiggle on entry?
    }
}
