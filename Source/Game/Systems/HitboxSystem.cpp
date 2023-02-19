#include "./HitboxSystem.h"
using namespace glm;

void HitboxSystem::Execute(
    Entity* entities,
    TransformComponent& transformComponent,
    HitboxComponent& hitboxComponent,
    BubbleComponent& bubbleComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponents<TransformComponent, HitboxComponent>())
            continue;
        const vec3 position = transformComponent.transform[i].position_;
        const float radius = bubbleComponent.radius[i];

        // TODO: Only check components within the same chunk
        // NOTE: There needs to be consistent checking between which components do what
        // basically skip double checks
        // Maybe do a i-j for loop on a chunk, with j starting at i instead of 0
        // Also should probably do hitbox scans in seperate systems, then handle resolution in other systems
        //
        // Flags may need to be cleared at the start of a frame
        for (int j = i + 1; j < MAX_ENTITIES; j++) {
            if (j == i)
                continue;
            const Entity& otherEntity = entities[i];
            if (!otherEntity.alive_)
                continue;
            if (!otherEntity.HasComponents<TransformComponent, HitboxComponent>())
                continue;
            const vec3 otherPosition = transformComponent.transform[j].position_;    
            const float otherRadius = bubbleComponent.radius[j]; 
            
            const float dist = distance(position, otherPosition);
            if (dist < radius + otherRadius) {
                // CODE: Check if the two components are in the states and set the flag
                // Set the state in this component only
                // Handle state actions in other component
                // Reset state 
                //
                // Have to check both orders, since intersection only does pairs once
                HandleIntersection(i, j, hitboxComponent);
                HandleIntersection(j, i, hitboxComponent);
            }
        }
    }
}

void HitboxSystem::HandleIntersection(
    int hitbox1,
    int hitbox2,
    HitboxComponent& hitboxComponent
) {
    // TODO: A pairing function and switch case may work better here
    if (hitboxComponent.properties[hitbox1][HitboxProperty::SendKick] && 
        hitboxComponent.properties[hitbox2][HitboxProperty::RecieveKick]) 
    {
        hitboxComponent.state[hitbox1].set(HitboxState::SentKick); 
        hitboxComponent.state[hitbox2].set(HitboxState::RecievedKick);
    }
}

