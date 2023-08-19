#pragma once
#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class EntityManager;
class SeedManager;
class IntervalSpawnComponent;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

namespace IntervalSpawnSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        EntityManager& entityManager, // TODO: Create a list for spawning instead
        SeedManager& seedManager,
        IntervalSpawnComponent& intervalSpawnComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
};
