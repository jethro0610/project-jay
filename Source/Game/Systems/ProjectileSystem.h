#pragma once
#include "../../Types/EntityTypes.h"
class EntityManager;
class World;

namespace ProjectileSystem {
    void CalculateVelocities(EntityManager& entityManager, World& world);
    void Launch(EntityManager& entityManager, EntityID projectileEntity);

    void Throw(
        EntityManager& entityManager,
        EntityID projectileEntity,
        EntityID throwingEntity,
        float height
    );
}
