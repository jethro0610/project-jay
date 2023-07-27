#pragma once
class Entity;
class EntityManager;
class SeedManager;
class IntervalSpawnComponent;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

namespace IntervalSpawnSystem {
    void Execute(
        Entity* entities,
        EntityManager& entityManager, // TODO: Create a list for spawning instead
        SeedManager& seedManager,
        IntervalSpawnComponent& intervalSpawnComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
};
