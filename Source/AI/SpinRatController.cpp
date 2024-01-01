#include "SpinRatController.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
#include "Particle/ParticleEmitter.h"
#include "Components/AIComponent.h"
#include "Components/GroundTraceComponent.h"
#include "Components/HitboxComponent.h"
#include "Components/HurtboxComponent.h"
#include "Components/MovementComponent.h"
#include "Components/SkeletonComponent.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Logging/Logger.h"
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

void SpinRatController::Init(   
    EntityID entityId, 
    EntityList& entities,
    ComponentList& components,
    Terrain& terrain
) {
    AIComponent& aiComponent = components.Get<AIComponent>(); 
    auto& hitboxComponent = components.Get<HitboxComponent>();
    MovementComponent& movementComponent = components.Get<MovementComponent>();
    SkeletonComponent& skeletonComponent = components.Get<SkeletonComponent>();
    TransformComponent& transformComponent = components.Get<TransformComponent>();

    Transform& transform = transformComponent.transform[entityId];
    vec3& desiredMovement = movementComponent.desiredMovement[entityId];

    vec3 centerOffset = RandomVectorPlanar(100.0f);
    desiredMovement = normalize(centerOffset - vec3(transform.position.x, 0.0f, transform.position.z));
    skeletonComponent.nextAnimationIndex[entityId] = 2;
}

void SpinRatController::Control(
    EntityID entityId,
    EntityList& entities,
    ComponentList& components,
    Terrain& terrain
) {
    auto& entity = entities[entityId];
    auto& aiComponent = components.Get<AIComponent>(); 
    auto& groundTraceComponent = components.Get<GroundTraceComponent>();
    auto& hitboxComponent = components.Get<HitboxComponent>();
    auto& hurtboxComponent = components.Get<HurtboxComponent>();
    auto& movementComponent = components.Get<MovementComponent>();
    auto& skeletonComponent = components.Get<SkeletonComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();

    Transform& transform = transformComponent.transform[entityId];
    vec3& velocity = velocityComponent.velocity[entityId]; 
    vec3& desiredMovement = movementComponent.desiredMovement[entityId];
    int& timer = aiComponent.timer[entityId];

    entity.emitters_[0]->active_ = false;
    if (groundTraceComponent.onGround[entityId])
        entity.emitters_[0]->active_ = true;

    if (timer > 0) {
        timer--;
        hitboxComponent.hitboxes[entityId][0].active = false;
    }
    else
        hitboxComponent.hitboxes[entityId][0].active = true;

    if (hurtboxComponent.stun[entityId]) {
        timer = 30;
        desiredMovement = -normalize(vec3(velocity.x, 0.0f, velocity.z));
        skeletonComponent.nextAnimationIndex[entityId] = 4;
        hitboxComponent.hitboxes[entityId][0].active = false;
        return;
    }
    else
        skeletonComponent.nextAnimationIndex[entityId] = 2;


    float curDistance = terrain.GetDistance(transform.position).x;
    float forwardDistance = terrain.GetDistance(transform.position + movementComponent.desiredMovement[entityId] * 10.0f).x;

    desiredMovement = rotate(desiredMovement, RandomFloatRange(-0.1f, 0.1f), Transform::worldUp);

    if (curDistance > -50.0f && forwardDistance - curDistance > 0.0f) {
        vec3 centerOffset = RandomVectorPlanar(100.0f);
        desiredMovement = normalize(centerOffset - vec3(transform.position.x, 0.0f, transform.position.z));
        timer = 0;
    }
}
