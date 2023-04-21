#pragma once
#include <tuple>
class Entity;
class EntityManager;
class TransformComponent;
class World;
class WorldColliderComponent;

const float GROUND_CUTOFF_SCALE = 0.25f;
namespace CollisionSystem {
    void Execute(
        Entity* entities,
        World& world, 
        TransformComponent& transformComponent,
        WorldColliderComponent& worldColliderComponent
    );
}
