#include <glm/vec3.hpp>
#include "Game.h"
#include "../Constants/GameConstants.h"
#include "../Constants/TimeConstants.h"
#include "Time.h"
using namespace glm;

using namespace std::chrono;
#define GETCOMP(COMP) entityManager_.GetComponent<COMP>()

void Game::Init() {
    // Create the camera and assign it to the renderer
    renderer_.camera_ = &camera_;

    resourceManager_.LoadStaticModel("st_sphere");
    // Create the player entity
    Transform spawnTransform;
    spawnTransform.position_ = vec3(0.0f, 50.0f, 0.0f);

    resourceManager_.LoadEntity("player");
    resourceManager_.LoadEntity("test_spawner");
    resourceManager_.LoadEntity("test_projectile");
    auto [playerEntityId, playerTransform] = entityManager_.CreateEntity("player");
    playerTransform = spawnTransform;

    camera_.trackEntity_ = PLAYER_ENTITY;

    // auto [holdEntityId, holdTransform] = entityManager_.CreateEntity("test_spawner");
    // holdTransform.position_ = vec3(0.0f, 40.0f, 0.0f);
    // holdTransform.scale_ = vec3(2.0f);
}

void Game::Update() {
    timeAccumlulator_ += Time::GetDeltaTime();
    while (timeAccumlulator_ >= TIMESTEP) {
        FlushInputs_P();
        IntervalSpawnSystem::Execute(
            entityManager_.entities_,
            entityManager_,
            GETCOMP(IntervalSpawnComponent),
            GETCOMP(ProjectileComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        TransformSystem::UpdateLastTransforms(
            entityManager_.entities_,
            GETCOMP(TransformComponent)
        );
        IntersectSystem::Execute(
            entityManager_.entities_,
            spreadManager_,
            GETCOMP(BubbleComponent),
            GETCOMP(ProjectileComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        SpreadActivatorSystem::Execute(
            entityManager_.entities_,
            spreadManager_,
            world_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(MeterComponent),
            GETCOMP(SpreadActivatorComponent),
            GETCOMP(SpreadDetectComponent),
            GETCOMP(TransformComponent) 
        );
        SpreadDetectSystem::Execute(
            entityManager_.entities_,
            spreadManager_,
            GETCOMP(SpreadDetectComponent),
            GETCOMP(TransformComponent)
        );
        playerController_.Execute(
            world_,
            spreadManager_,
            camera_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(MeterComponent),
            GETCOMP(MovementComponent),
            GETCOMP(SpreadActivatorComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent),
            inputs_
        );
        MovementSystem::Execute(
            entityManager_.entities_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(MovementComponent),
            GETCOMP(SpreadDetectComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        ProjectileSystem::CalculateVelocities(
            entityManager_.entities_,
            world_,
            GETCOMP(ProjectileComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent) 
        );
        GroundStickSystem::Step(
            entityManager_.entities_,
            world_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(TransformComponent)
        );
        VelocitySystem::Apply(
            entityManager_.entities_,
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        GroundTraceSystem::Execute(
            entityManager_.entities_,
            world_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        GroundStickSystem::Stick(
            entityManager_.entities_,
            world_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(TransformComponent)
        );
        CollisionSystem::Execute(
            entityManager_.entities_, 
            world_, 
            GETCOMP(TransformComponent),
            GETCOMP(WorldColliderComponent)
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(
        entityManager_.entities_,
        GETCOMP(TransformComponent),
        timeAccumlulator_
    );
    seedManager_.Execute(
        GETCOMP(MeterComponent),
        GETCOMP(TransformComponent)
    );
    world_.UpdateDirtyChunks();
    spreadManager_.UpdateRenderData_P();
    camera_.Update(inputs_);
    renderer_.Render(
        entityManager_.entities_, 
        seedManager_,
        spreadManager_, 
        world_, 
        GETCOMP(MeterComponent),
        GETCOMP(StaticModelComponent),
        GETCOMP(TransformComponent)
    );
}
