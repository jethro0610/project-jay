#include "KickSystem.h"
using namespace glm;

void KickSystem::Execute(
    Entity* entities,
    TransformComponent& transformComponent,
    HitboxComponent& hitboxComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponents<TransformComponent, HitboxComponent>())
            continue;
        if (!hitboxComponent.flags[i][HitboxFlag::SendKick])
            continue;
        const vec3 position = transformComponent.transform[i].position_;
        const float radius = hitboxComponent.radius[i];

        // TODO: Only check components within the same chunk
        // NOTE: There needs to be consistent checking between which components do what
        // basically skip double checks
        // Maybe do a i-j for loop on a chunk, with j starting at i instead of 0
        // Also should probably do hitbox scans in seperate systems, then handle resolution in other systems
        for (int j = 0; j < MAX_ENTITIES; j++) {
            if (j == i)
                continue;
            const Entity& otherEntity = entities[i];
            if (!otherEntity.alive_)
                continue;
            if (!otherEntity.HasComponents<TransformComponent, HitboxComponent>())
                continue;
            if (!hitboxComponent.flags[j][HitboxFlag::RecieveKick])
                continue;

            const vec3 otherPosition = transformComponent.transform[j].position_;    
            const float otherRadius = hitboxComponent.radius[j]; 
            
            const float dist = distance(position, otherPosition);
            if (dist < radius + otherRadius) {
                transformComponent.transform[j].position_ = vec3(0.0f); 
            }
        }
    }
}
