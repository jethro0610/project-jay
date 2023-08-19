#pragma once
#include <array>
#include <glm/vec3.hpp>
#include <vector>
#include "../Entity/EntityID.h"
#include "../Entity/EntityLimits.h"
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
    std::array<Entity, MAX_ENTITIES>& entities;
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
    void GetCollisions(
        CollisionArgs args,
        std::vector<Collision>& collisions
    ); 

    void HandleCollisions(
        CollisionArgs args,
        Collision* collisions,
        int numCollisions
    );

    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        ColliderComponent& colliderComponent,
        MeterComponent& meterComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
};
