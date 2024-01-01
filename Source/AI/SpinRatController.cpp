#include "SpinRatController.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
#include "Particle/ParticleEmitter.h"
#include "Components/AIComponent.h"
#include "Components/MovementComponent.h"
#include "Components/SkeletonComponent.h"
#include "Components/TransformComponent.h"
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
    Entity& entity = entities[entityId];
    AIComponent& aiComponent = components.Get<AIComponent>(); 
    MovementComponent& movementComponent = components.Get<MovementComponent>();
    SkeletonComponent& skeletonComponent = components.Get<SkeletonComponent>();
    TransformComponent& transformComponent = components.Get<TransformComponent>();

    Transform& transform = transformComponent.transform[entityId];
    vec3& desiredMovement = movementComponent.desiredMovement[entityId];
    int& timer = aiComponent.timer[entityId];

    float curDistance = terrain.GetDistance(transform.position).x;
    float forwardDistance = terrain.GetDistance(transform.position + movementComponent.desiredMovement[entityId] * 10.0f).x;

    desiredMovement = rotate(desiredMovement, RandomFloatRange(-0.1f, 0.1f), Transform::worldUp);

    if (timer < 45) {
        entity.emitters_[0]->active_ = false;
        skeletonComponent.nextAnimationIndex[entityId] = 2;
        timer++;
    }
    else {
        entity.emitters_[0]->active_ = true;
        skeletonComponent.nextAnimationIndex[entityId] = 2;
    }

    if (curDistance > -50.0f && forwardDistance - curDistance > 0.0f) {
        vec3 centerOffset = RandomVectorPlanar(100.0f);
        desiredMovement = normalize(centerOffset - vec3(transform.position.x, 0.0f, transform.position.z));
        timer = 0;
    }
}
