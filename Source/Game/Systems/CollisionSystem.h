#pragma once
#include "../Entity/EntityID.h"
#include <glm/vec3.hpp>
class Entity;
class SeedManager;
class SpreadManager;
class ColliderComponent;
class MeterComponent;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

const int MAX_COLLISIONS = 128;

struct CollisionArgs {
    Entity* entities;
    SeedManager& seedManager;
    SpreadManager& spreadManager;
    ColliderComponent& colliderComponent;
    MeterComponent& meterComponent;
    ProjectileComponent& projectileComponent;
    TransformComponent& transformComponent;
    VelocityComponent& velocityComponent;
};

struct Collision {
    EntityID entity1;
    EntityID entity2;
    glm::vec3 resolutionVec;
};

namespace CollisionSystem {
    int GetCollisions(
        CollisionArgs args,
        Collision* collisions
    ); 

    void HandleCollisions(
        CollisionArgs args,
        Collision* collisions,
        int numCollisions
    );

    void Execute(
        Entity* entities,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        ColliderComponent& colliderComponent,
        MeterComponent& meterComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
};
