#include "WalkerController.h"
#include "Components/HitboxComponent.h"
#include "Components/MovementComponent.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Particle/ParticleManager.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
using namespace glm;

void WalkerController::Init(
    EntityID entity, 
    ComponentList& components,
    ParticleManager& particleManager,
    Terrain& terrain 
) {
    auto& transformComponent = components.Get<TransformComponent>();

    stopTime_ = 0.0f;
    entity_ = entity;
    targetPosition_ = vec2(
        transformComponent.transform[entity_].position.x,
        transformComponent.transform[entity_].position.z
    );
    // cloudEmitter_ = particleManager.RequestEmitter("p_cloud");
    // cloudEmitter_->parent_ = &transformComponent.transform[entity_];

    FindNewTarget(terrain);
}

void WalkerController::Update(
    ComponentList& components,
    Terrain& terrain
) {
    auto& movementComponent = components.Get<MovementComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();
    auto& hitboxComponent = components.Get<HitboxComponent>();

    vec3& position = transformComponent.transform[entity_].position; 
    vec3& desiredMovement = movementComponent.desiredMovement[entity_];
    // cloudEmitter_->velocityOffset_ = velocityComponent.velocity[entity_] * 0.75f;
    // cloudEmitter_->active_ = false;

    if (distance(vec2(position.x, position.z), targetPosition_) < 1.0f && stopTime_ <= 0.0f) {
        FindNewTarget(terrain);
        stopTime_ = 5.0f;
    }

    if (stopTime_ > 0.0f) {
        hitboxComponent.hitboxes[entity_][0].active = false;
        desiredMovement = vec3(0.0f);
        stopTime_ -= GlobalTime::TIMESTEP;
        return;
    }
    // cloudEmitter_->active_ = true;

    desiredMovement = vec3(targetPosition_.x - position.x, 0.0f, targetPosition_.y - position.z);
    if (length(desiredMovement) > 0.0f)
        desiredMovement = normalize(desiredMovement);

    hitboxComponent.hitboxes[entity_][0].active = true; 
}

void WalkerController::FindNewTarget(Terrain& terrain) {
    vec2 initPos = targetPosition_;
    do {
        targetPosition_ = initPos + RandomVector2D(MIN_WALK_DISTANCE, MAX_WALK_DISTANCE);
    }
    while (
        any(isnan(targetPosition_)) ||
        terrain.GetDistance(targetPosition_).x > -128.0f
    );
}