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

        float time = detectComponent[i].time;
        ivec2 currentKey = spreadManager->WorldPositionToSpreadKey(transformComponent.transform[i].position_);
        if (spreadManager->SpreadIsActive(currentKey))
            time += TIMESTEP; 

        detectComponent.time[i] = clamp(time, 0.0f, 1.0f);
        // TODO: Only increment time on ENTRY of spread, not on stay.
        //
        // Have an array of passed spread points (probably last 8)
        // Test if the current is in the array
        // If it isn't, then decrease the speed
        //
        // The activator component should add to the array BEFORE execution that way
        // activated spread isn't counted
        //
        // Maybe have the spread wiggle on entry?
    }
}
