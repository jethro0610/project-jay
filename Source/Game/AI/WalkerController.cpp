#include "WalkerController.h"
#include "../../Constants/TimeConstants.h"
#include "../../Helpers/Random.h"
#include "../World/World.h"
#include "../Components/MovementComponent.h"
#include "../Components/SkeletonComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../ParticleManager.h"
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

void WalkerController::Init(
    EntityID entity, 
    ParticleManager& particleManager,
    World& world, 
    TransformComponent& transformComponent
) {
    stopTime_ = 0.0f;
    entity_ = entity;
    targetPosition_ = vec2(
        transformComponent.transform[entity_].position.x,
        transformComponent.transform[entity_].position.z
    );
    cloudEmitter_ = particleManager.RequestEmitter("p_cloud");
    cloudEmitter_->parent_ = &transformComponent.transform[entity_];

    FindNewTarget(world);
}

void WalkerController::Update(
    World& world,
    MovementComponent& movementComponent,
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    vec3& position = transformComponent.transform[entity_].position; 
    vec3& desiredMovement = movementComponent.desiredMovement[entity_];
    cloudEmitter_->velocityOffset_ = velocityComponent.velocity[entity_] * 0.75f;
    cloudEmitter_->active_ = false;

    if (distance(vec2(position.x, position.z), targetPosition_) < 1.0f && stopTime_ <= 0.0f) {
        FindNewTarget(world);
        stopTime_ = 5.0f;
    }

    if (stopTime_ > 0.0f) {
        desiredMovement = vec3(0.0f);
        stopTime_ -= TIMESTEP;
        return;
    }
    cloudEmitter_->active_ = true;

    desiredMovement = vec3(targetPosition_.x - position.x, 0.0f, targetPosition_.y - position.z);
    if (length(desiredMovement) > 0.0f)
        desiredMovement = normalize(desiredMovement);
}

void WalkerController::FindNewTarget(World& world) {
    vec2 initPos = targetPosition_;
    do {
        targetPosition_ = initPos + RandomVector2D(MIN_WALK_DISTANCE, MAX_WALK_DISTANCE);
    }
    while (
        any(isnan(targetPosition_)) ||
        world.GetDistance(targetPosition_).x > -128.0f
    );
}
