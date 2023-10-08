#pragma once
#include <glm/vec2.hpp>
#include "../Entity/EntityID.h"

class World;
class ParticleEmitter;
class ParticleManager;
class MovementComponent;
class SkeletonComponent;
class TransformComponent;
class VelocityComponent;

// Enemy will walk around for a little bit
// When spread is dectected in radius, it will bust and remove a lot
// Can only hit in cooldown state
// Will sniff then bust (its blind)

const float MIN_WALK_DISTANCE = 50.0f;
const float MAX_WALK_DISTANCE = 100.0f;

class WalkerController {
public:
    EntityID entity_;
    glm::vec2 targetPosition_;
    float stopTime_;
    ParticleEmitter* cloudEmitter_;

    void Init(
        EntityID entity, 
        ParticleManager& particleManager,
        World& world, 
        TransformComponent& transformComponent
    );

    void Update(
        World& world,
        MovementComponent& movementComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );

    void FindNewTarget(World& world);
};
