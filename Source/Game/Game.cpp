#include "Game.h"
#include "Time/Time.h"
#include <glm/vec3.hpp>
#include <ctime>
using namespace glm;

using namespace std::chrono;

void Game::Init() {
    srand(time(0));
    entityManager_.components_.Get<SkeletonComponent>().resourceManager = &resourceManager_;
    entityManager_.components_.Get<StaticModelComponent>().resourceManager = &resourceManager_;
    entityManager_.components_.Get<IntervalSpawnComponent>().resourceManager = &resourceManager_;

    // Create the camera and assign it to the renderer
    renderer_.camera_ = &camera_;

    camera_.target_ = PLAYER_ENTITY;

    entityListS_.CreateEntity(Player::GetTypeID());
    entityListS_[0].transform_.position = vec3(0.0f, 10.0f, 0.0f);

    entityListS_.CreateEntity(BumpRat::GetTypeID());
    entityListS_[1].transform_.position = vec3(10.0f, 10.0f, 10.0f);

    #ifdef _DEBUG
    editor_.StartEditing();
    #endif
}

void Game::Update() {
    timeAccumlulator_ += GlobalTime::GetDeltaTime();
    while (timeAccumlulator_ >= GlobalTime::TIMESTEP) {
        FlushInputs_P();
        #ifdef _DEBUG
        if (editor_.IsActive()) {
            timeAccumlulator_ = 0.0f;
            return;
        }
        #endif

        for (int i = 0; i < 128; i++) {
            if (!entityListS_.Valid(i)) continue;
            entities_[i].entity.lastTransform_ = entities_[i].entity.transform_;
            switch(entities_[i].typeId) {
                #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): entities_[i].VAR.Update(); break;
                EXPANDENTITIES
                #undef ENTITYEXP
            }
            entities_[i].entity.BaseUpdate();
        }

        entityManager_.DestroyEntities();
        entityManager_.SpawnEntities();
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
            entityManager_.components_,
            seedManager_
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
            terrain_,
            spreadManager_,
            camera_,
            inputs_
        );
        AISystem::InitAIs(
            entityManager_.entities_,
            entityManager_.components_,
            terrain_
        );
        AISystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            terrain_
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
            terrain_
        );
        GroundStickSystem::Stick(
            entityManager_.entities_,
            entityManager_.components_,
            terrain_ 
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
            terrain_
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
            entityManager_.spawner_,
            seedManager_
        );
        SeedOnDestroySystem::Execute(
            entityManager_.entities_,
            entityManager_.components_,
            entityManager_.destroyList_,
            seedManager_
        );
        entityManager_.ClearEntitySpawnFlags();
        timeAccumlulator_ -= GlobalTime::TIMESTEP;
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
        terrain_,
        timeAccumlulator_
    );
    seedManager_.GetCaptures(
        entityManager_.entities_,
        entityManager_.components_
    );
    camera_.Update(inputs_);

    for (int i = 0; i < 128; i++) {
        if (!entityListS_.Valid(i)) continue;
        entities_[i].entity.BaseRenderUpdate(timeAccumlulator_ / GlobalTime::TIMESTEP);
        switch(entities_[i].typeId) {
            #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): entities_[i].VAR.RenderUpdate(); break;
            EXPANDENTITIES
            #undef ENTITYEXP
        }
    }

    ParticleAttachSystem::Execute(
        entityManager_.entities_,
        entityManager_.components_
    );
    particleManager_.Update(GlobalTime::GetDeltaTime());
    renderer_.Render(
        entityManager_.entities_, 
        entityManager_.components_,
        levelProperties_,
        particleManager_,
        seedManager_,
        spreadManager_, 
        terrain_,
        entityListS_
    );
}
