#include "Game.h"
#include "../Constants/TimeConstants.h"
#include "../Logging/Logger.h"

using namespace std::chrono;

void Game::Init() {
    // Initialize the time
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    lastTimeUSec_ = currentTimeUSec_;
    elapsedTime_ = 0.0f;

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

    auto [holdEntityId, holdTransform] = entityManager_.CreateEntity("test_spawner");
    holdTransform.position_ = vec3(0.0f, 40.0f, 0.0f);
    holdTransform.scale_ = vec3(2.0f);
}

void Game::Update(float deltaTime, float elapsedTime) {
    timeAccumlulator_ += deltaTime;
    while (timeAccumlulator_ >= TIMESTEP) {
        FlushInputs_P();
        IntervalSpawnSystem::Execute(
            entityManager_,
            entityManager_.transformComponent_,
            entityManager_.intervalSpawnComponent_,
            entityManager_.projectileComponent_,
            entityManager_.velocityComponent_
        );
        TransformSystem::UpdateLastTransforms(
            entityManager_,
            entityManager_.transformComponent_
        );
        IntersectSystem::Execute(
            entityManager_,
            spreadManager_
        );
        PickupSystem::ExecuteHold(
            entityManager_,
            entityManager_.pickupComponent_,
            entityManager_.transformComponent_,
            entityManager_.velocityComponent_,
            entityManager_.projectileComponent_
        );
        SpreadActivatorSystem::Execute(
            entityManager_,
            world_,
            spreadManager_
        );
        SpreadDetectSystem::Execute(
            entityManager_, 
            spreadManager_
        );
        playerController_.Execute(
            inputs_ 
        );
        MovementSystem::Execute(
            entityManager_,
            entityManager_.movementComponent_,
            entityManager_.groundTraceComponent_,
            entityManager_.transformComponent_,
            entityManager_.velocityComponent_,
            entityManager_.spreadDetectComponent_
        );
        ProjectileSystem::CalculateVelocities(
            entityManager_,
            world_,
            entityManager_.projectileComponent_,
            entityManager_.velocityComponent_,
            entityManager_.transformComponent_
        );
        GroundStickSystem::Step(
            entityManager_,
            world_,
            entityManager_.transformComponent_,
            entityManager_.groundTraceComponent_
        );
        VelocitySystem::Apply(
            entityManager_,
            entityManager_.transformComponent_,
            entityManager_.velocityComponent_
        );
        GroundTraceSystem::Execute(
            entityManager_,
            world_
        );
        GroundStickSystem::Stick(
            entityManager_,
            world_,
            entityManager_.transformComponent_,
            entityManager_.groundTraceComponent_
        );
        CollisionSystem::Execute(
            entityManager_,
            world_, 
            entityManager_.transformComponent_,
            entityManager_.worldColliderComponent_
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(
        timeAccumlulator_, 
        entityManager_,
        entityManager_.transformComponent_
    );
    world_.UpdateDirtyChunks();
    spreadManager_.UpdateRenderData_P();
    camera_.Update(deltaTime, inputs_);
    RenderComponents renderComponents {
        entityManager_.staticModelComponent_,
        entityManager_.transformComponent_
    };
    renderer_.Render(
        deltaTime, 
        elapsedTime, 
        entityManager_.entities_, 
        renderComponents, spreadManager_,
        world_
    );
}

void Game::UpdateTime() {
    lastTimeUSec_ = currentTimeUSec_;
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    deltaTime_ = (currentTimeUSec_ - lastTimeUSec_) * 0.000001f;
    elapsedTime_ += deltaTime_;
    deltaTime_ = min<float>(MAX_DELTA_TIME, deltaTime_);
}
