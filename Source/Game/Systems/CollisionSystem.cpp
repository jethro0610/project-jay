#include "CollisionSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/World.h"
#include "../Components/TransformComponent.h"
#include "../Components/WorldColliderComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<TransformComponent, WorldColliderComponent>();

void CollisionSystem::Execute(
    Entity* entities, 
    World& world, 
    TransformComponent& transformComponent, 
    WorldColliderComponent& worldColliderComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        // Raymarch towards the nearest point on the surface
        vec3 position = transformComponent.transform[i].position;
        float worldHeight = world.GetHeight(position);
        float distance = position.y - worldHeight;

        worldColliderComponent.colliding[i] = distance <= 0.05f;

        // // Hit solving
        // if (distance <= 0.001f) {
        //     vec3 hitNormal = world.GetNormal(position);
        //     transformComponent.transform[i].position += hitNormal * abs(distance);
        // }
    }
}
