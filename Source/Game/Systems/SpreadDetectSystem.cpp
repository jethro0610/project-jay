#include "SpreadDetectSystem.h"
using namespace glm;

void SpreadDetectSystem::Execute(
    EntityManager& entityManager, 
    SpreadManager& spreadManager
) {
    SpreadDetectComponent& detectComponent = entityManager.spreadDetectComponent_;
    SpreadActivatorComponent& activatorComponent = entityManager.spreadActivatorComponent_;
    TransformComponent& transformComponent = entityManager.transformComponent_;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponents({detectComponent, transformComponent}))
            continue;

        bool& detected = detectComponent.deteced[i];
        detected = false;

        ivec2 currentKey = spreadManager.WorldPositionToSpreadKey(transformComponent.transform[i].position_);
        if (!spreadManager.SpreadIsActive(currentKey)) {
            detectComponent.lastAdd[i] = NO_ADD;
            continue;
        }

        if (currentKey == detectComponent.lastKey[i])
            continue;
        detectComponent.lastKey[i] = currentKey;

        const bool hasActivator = entity.HasComponent(activatorComponent);
        if (hasActivator && distance(vec2(detectComponent.lastAdd[i]), vec2(currentKey)) <= MAX_ADD_DISTANCE)
            continue;

        detected = true;
        detectComponent.lastAdd[i] = NO_ADD;  
        // Maybe have the spread wiggle on entry?
    }
}
