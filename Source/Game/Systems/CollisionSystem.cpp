#include "CollisionSystem.h"
#include "../Entity/EntityManager.h"
#include "../World/World.h"
using namespace glm;

void CollisionSystem::Execute(EntityManager& entityManager, World& world) {
    TransformComponent& transformComponent = entityManager.transformComponent_;
    WorldColliderComponent& worldColliderComponent = entityManager.worldColliderComponent_;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponents({transformComponent, worldColliderComponent}))
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
