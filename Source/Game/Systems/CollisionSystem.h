#pragma once
class EntityManager;
class World;

const float GROUND_CUTOFF_SCALE = 0.25f;

namespace CollisionSystem {
    void Execute(EntityManager& entityManager, World& world);
}
