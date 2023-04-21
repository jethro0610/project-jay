#pragma once
class Entity;
class EntityManager;
class IntervalSpawnComponent;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

namespace IntervalSpawnSystem {
    void Execute(
        Entity* entities,
        EntityManager& entityManager, // TODO: Create a list for spawning instead
        IntervalSpawnComponent& intervalSpawnComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
};
