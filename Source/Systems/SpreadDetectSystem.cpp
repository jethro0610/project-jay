#include "SpreadDetectSystem.h"
#include "Components/SpreadDetectComponent.h"
#include "Components/TransformComponent.h"
#include "Spread/SpreadManager.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<SpreadDetectComponent, TransformComponent>();

void SpreadDetectSystem::Execute(
    EntityList& entities,
    ComponentList& components,
    SpreadManager& spreadManager
) {
    auto& spreadDetectComponent = components.Get<SpreadDetectComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;

        bool& detected = spreadDetectComponent.deteced[i];
        detected = false;

        SpreadKey currentKey = spreadManager.GetKey(transformComponent.transform[i].position);
        if (!spreadManager.SpreadIsActive(currentKey)) {
            spreadDetectComponent.lastAdd[i] = SpreadDetectComponent::NO_ADD;
            continue;
        }

        if (currentKey == spreadDetectComponent.lastKey[i])
            continue;
        spreadDetectComponent.lastKey[i] = currentKey;

        if (
            spreadDetectComponent.lastAdd[i] != SpreadDetectComponent::NO_ADD && 
            distance(vec2(spreadDetectComponent.lastAdd[i]), vec2(currentKey)) <= SpreadDetectComponent::MAX_ADD_DISTANCE
        ) continue;

        detected = true;
        spreadDetectComponent.lastAdd[i] = SpreadDetectComponent::NO_ADD;  
        // Maybe have the spread wiggle on entry?
    }
}