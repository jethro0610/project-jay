#include "SpreadDetectSystem.h"
#include "../TimeConstants.h"
using namespace glm;

void SpreadDetectSystem::Execute(
    Entity* entities, 
    SpreadManager* spreadManager,
    TransformComponent& transformComponent,
    SpreadDetectComponent& detectComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponents<SpreadDetectComponent, TransformComponent>())
            continue;

        bool& detectedSpread = detectComponent.detecedSpread[i];
        detectedSpread = false;
        ivec2* lastDetect = detectComponent.lastDetect[i];
        ivec2 currentKey = spreadManager->WorldPositionToSpreadKey(transformComponent.transform[i].position_);
        if (!spreadManager->SpreadIsActive(currentKey))
            continue;

        // Check if the spread has already been detected
        bool touchedNew = true;
        for (int s = 0; s < MAX_DETECT; s++) {
            if (lastDetect[s] == currentKey) {
                touchedNew = false;
                break;
            }
        }
        if (!touchedNew)
            continue;
        
        // Pop the queue
        for (int s = 0; s < MAX_DETECT - 1; s++) 
            lastDetect[s + 1] = lastDetect[s];
        lastDetect[0] = currentKey;
        detectedSpread = true;

        // The activator component should add to the array BEFORE execution that way
        // activated spread isn't counted
        //
        // Maybe have the spread wiggle on entry?
    }
}
