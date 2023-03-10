#include "./IntersectSystem.h"
#include "../Systems/PickupSystem.h"
using namespace glm;

void IntersectSystem::Execute(
    EntityManager& entityManager,
    SpreadManager& spreadManager,
    TransformComponent& transformComponent,
    BubbleComponent& bubbleComponent,
    PickupComponent& pickupComponent,
    HoldableComponent& holdableComponent,
    KickerComponent& kickerComponent,
    KickableComponent& kickableComponent
) {
    const Entity* entities = entityManager.entities_;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponents({transformComponent, bubbleComponent}))
            continue;
        const vec3 position = transformComponent.transform[i].position_;
        const float radius = bubbleComponent.radius[i];

        // TODO: Only check components within the same chunk
        // NOTE: There needs to be consistent checking between which components do what
        // basically skip double checks
        // Maybe do a i-j for loop on a chunk, with j starting at i instead of 0
        for (int j = i + 1; j < MAX_ENTITIES; j++) {
            if (j == i)
                continue;

            const Entity& otherEntity = entities[j];
            if (!otherEntity.alive_)
                continue;
            if (!entity.HasComponents({transformComponent, bubbleComponent}))
                continue;

            const vec3 otherPosition = transformComponent.transform[j].position_;    
            const float otherRadius = bubbleComponent.radius[j]; 
            
            const float dist = distance(position, otherPosition);
            if (dist < radius + otherRadius) {
                HandleIntersection(
                    entityManager,
                    spreadManager, 
                    i, 
                    j, 
                    transformComponent,
                    pickupComponent, 
                    holdableComponent, 
                    kickerComponent, 
                    kickableComponent
                );

                HandleIntersection(
                    entityManager,
                    spreadManager, 
                    j, 
                    i, 
                    transformComponent,
                    pickupComponent, 
                    holdableComponent, 
                    kickerComponent, 
                    kickableComponent
                );
            }
        }
    }
}

void IntersectSystem::HandleIntersection(
    EntityManager& entityManager,
    SpreadManager& spreadManager,
    int hitbox1,
    int hitbox2,
    TransformComponent& transformComponent,
    PickupComponent& pickupComponent,
    HoldableComponent& holdableComponent,
    KickerComponent& kickerComponent,
    KickableComponent& kickableComponent
) {
    const Entity* entities = entityManager.entities_;
    if (entities[hitbox1].HasComponent(pickupComponent) && 
        entities[hitbox2].HasComponent(holdableComponent)) 
    {
        PickupSystem::DoPickup(hitbox1, hitbox2, pickupComponent);
    }
    
    if (entities[hitbox1].HasComponent(kickerComponent) && 
        entities[hitbox2].HasComponent(kickableComponent)) 
    {
        spreadManager.AddSpread(transformComponent.transform[hitbox2].position_, 8);
        entityManager.DestroyEntity(hitbox2);
    }
}
