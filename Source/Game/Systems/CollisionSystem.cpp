#include "CollisionSystem.h"
#include "../Entity/Entity.h"
#include "../../Helpers/EntityHelpers.h"
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
        vec3 position = transformComponent.transform[i].position_;
        vec3 directionToSurface = -world.GetNormal(position, 2.0f);
        float distance = world.GetDistance(position);

        worldColliderComponent.colliding[i] = distance <= 0.05f;

        // Hit solving
        if (distance <= 0.001f) {
            vec3 hitNormal = world.GetNormal(position);
            transformComponent.transform[i].position_ += hitNormal * abs(distance);
        }
    }
}
