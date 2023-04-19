#pragma once
class EntityManager;
class World;

namespace GroundStickSystem {
    void Step(EntityManager& entityManager, World& world);
    void Stick(EntityManager& entityManager, World& world);
}
