#include "IntersectSystem.h"
#include "ProjectileSystem.h"
#include "../Entity/Entity.h"
#include "../../Helpers/EntityHelpers.h"
#include "../World/SpreadManager.h"
#include "../Components/BubbleComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Constants/GameConstants.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<BubbleComponent, TransformComponent>();

void IntersectSystem::Execute(
    Entity* entities,
    SpreadManager& spreadManager,
    BubbleComponent& bubbleComponent,
    ProjectileComponent& projectileComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
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
            if (!otherEntity.MatchesKey(key))
                continue;

            const vec3 otherPosition = transformComponent.transform[j].position_;    
            const float otherRadius = bubbleComponent.radius[j]; 
            
            const float dist = distance(position, otherPosition);
            if (dist < radius + otherRadius) {
                // TODO: Store the intersections instead
                HandleIntersection(
                    entities, 
                    spreadManager, 
                    bubbleComponent, 
                    projectileComponent, 
                    transformComponent, 
                    velocityComponent, 
                    i, 
                    j
                );
                HandleIntersection(
                    entities, 
                    spreadManager, 
                    bubbleComponent, 
                    projectileComponent, 
                    transformComponent, 
                    velocityComponent, 
                    j, 
                    i
                );
            }
        }
    }
}

constexpr EntityKey projectileKey = GetEntityKey<ProjectileComponent, VelocityComponent>();

void IntersectSystem::HandleIntersection(
    Entity* entities,
    SpreadManager& spreadManager,
    BubbleComponent& bubbleComponent,
    ProjectileComponent& projectileComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent,
    EntityID entity1,
    EntityID entity2 
) {
    if (bubbleComponent.properties[entity1].test(BubbleProperties::Meteor)) {
        const vec3 velocity = velocityComponent.velocity[entity1];
        float speed = length(vec2(velocity.x, velocity.z));
        if (
            speed >= METEOR_SPEED &&
            bubbleComponent.properties[entity2].test(BubbleProperties::ThrowOnMeteored) && 
            projectileComponent.state[entity2] == ProjectileState::Inactive
        ) {
            ProjectileSystem::Throw(entities, projectileComponent, transformComponent, velocityComponent, entity2, entity1, 30.0f); 
        }
    }
}
