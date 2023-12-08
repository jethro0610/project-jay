#include <glm/vec3.hpp>
#include <ctime>
#include "Game.h"
#include "../Constants/GameConstants.h"
#include "../Constants/TimeConstants.h"
#include "Time.h"
#include "../Logging/Logger.h"
#include "../Helpers/Random.h"
using namespace glm;

using namespace std::chrono;

void Game::Init() {
    srand(time(0));
    entityManager_.components_.Get<SkeletonComponent>().resourceManager = &resourceManager_;
    entityManager_.components_.Get<StaticModelComponent>().resourceManager = &resourceManager_;
    entityManager_.components_.Get<IntervalSpawnComponent>().resourceManager = &resourceManager_;

    resourceManager_.LoadEmitterProperties("p_sparkle");
    resourceManager_.LoadEmitterProperties("p_dust");
    resourceManager_.LoadEmitterProperties("p_cloud");
    resourceManager_.LoadEmitterProperties("p_spark");

    // Create the camera and assign it to the renderer
    renderer_.camera_ = &camera_;

    // Create the player entity
    resourceManager_.LoadEntityDescription("e_player");
    resourceManager_.LoadEntityDescription("e_tpillar");
    resourceManager_.LoadEntityDescription("e_comet_spawner");
    resourceManager_.LoadEntityDescription("e_comet");
    levelLoader_.LoadLevel("test_level");

    camera_.target_ = PLAYER_ENTITY;
}

void Game::Update() {
    timeAccumlulator_ += GlobalTime::GetDeltaTime();
    while (timeAccumlulator_ >= TIMESTEP) {
        FlushInputs_P();
        entityManager_.SpawnEntities();
        entityManager_.DestroyEntities();
        FreezeSytem::Execute(entityManager_.entities_);
        TransformSystem::UpdateLastTransforms(
            entityManager_.entities_,
            entityManager_.components_
        );
        PushSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_
        );
        HitSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_
        );
        SpreadDetectSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            spreadManager_
        );
        // testWalker.Update(
        //     entityManager_.components_,
        //     world_
        // );
        playerController_.Execute(
            entityManager_.entities_,
            entityManager_.components_,
            world_,
            spreadManager_,
            camera_,
            inputs_
        );
        VelocitySystem::CalculateGravity(
            entityManager_.entities_,
            entityManager_.components_
        );
        MovementSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_
        );
        ProjectileSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_
        );
        GroundTraceSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            entityManager_.destroyList_,
            world_
        );
        GroundStickSystem::Stick(
            entityManager_.entities_,
            entityManager_.components_,
            world_
        );
        VelocitySystem::Apply(
            entityManager_.entities_,
            entityManager_.components_
        );
        TrampleSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            spreadManager_
        );
        SpreadActivatorSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            spreadManager_,
            world_
        );
        SkeletonSystem::CalculatePoses(
            entityManager_.entities_,
            entityManager_.components_
        );
        DestroyMeterSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            entityManager_.destroyList_
        );
        DestroyOnBoundSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            entityManager_.destroyList_
        );
        IntervalSpawnSystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            entityManager_.spawnList_,
            seedManager_
        );
        SpawnSystem::ClearSpawns(
            entityManager_.entities_
        );
        SeedOnDestroySystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            entityManager_.destroyList_,
            seedManager_
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    SkeletonSystem::InterpPoses(
        entityManager_.entities_,
        entityManager_.components_,
        timeAccumlulator_
    );
    TransformSystem::UpdateRenderTransforms(
        entityManager_.entities_,
        entityManager_.components_,
        timeAccumlulator_
    );
    seedManager_.CalculatePositions(
        entityManager_.components_,
        world_,
        timeAccumlulator_
    );
    seedManager_.GetCaptures(
        entityManager_.entities_,
        entityManager_.components_
    );
    camera_.Update(inputs_);
    ParticleAttachSystem::Execute(
        entityManager_.entities_,
        entityManager_.components_
    );
    particleManager_.Update(GlobalTime::GetDeltaTime());
    renderer_.Render(
        entityManager_.entities_, 
        entityManager_.components_,
        particleManager_,
        seedManager_,
        spreadManager_, 
        world_
    );
}
