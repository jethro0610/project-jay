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

struct HitS {
    EntityS* hitter;
    EntityS* target;
    Collision collision;
};

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
            if (!entityListS_[i].GetFlag(EntityS::EF_SendHits) && !entities_[i].entity.GetFlag(EntityS::EF_RecieveHits))
                continue;
        
            entityListS_[i].hit_ = false;
            entityListS_[i].hurt_ = false;
        }

        vector_const<HitS, 128> hitList;
        for (int h = 0; h < MAX_ENTITIES; h++) {
            if (!entityListS_.Valid(h)) continue;
            if (!entityListS_[h].GetFlag(EntityS::EF_SendHits))
                continue;
            EntityS& hitter = entityListS_[h];

            for (int t = 0; t < MAX_ENTITIES; t++) {
                if (h == t) continue;
                if (!entityListS_.Valid(t)) continue;
                if (!entityListS_[t].GetFlag(EntityS::EF_RecieveHits)) continue;
                EntityS& target = entityListS_[t];

                Collision collision = Collision::GetCollision(
                    hitter.transform_, 
                    hitter.hitbox_, 
                    target.transform_, 
                    target.hurtbox_
                );
                if (!collision.isColliding)
                    continue;

                vec3 planarForward = entityListS_[h].transform_.GetForwardVector();
                planarForward.y = 0.0f;
                planarForward = normalize(planarForward);

                vec3 vectorToTarget = target.transform_.position - hitter.transform_.position;
                vectorToTarget.y = 0.0f;
                vectorToTarget = normalize(vectorToTarget);

                if (dot(planarForward, vectorToTarget) > hitter.hitbox_.forwardRange)
                    hitList.push_back({&hitter, &target, collision});
            }
        }

        for (const HitS& hit : hitList) {
            // groundTraceComponent.disableStick[hit.target] = true;
            // hurtboxComponent.stunTimer[hit.target] = 0;
            // entities[hit.hitter].StartHitlag(hitbox.hitlag, false);
            // entities[hit.target].StartHitlag(hitbox.hitlag, true);
            // hurtboxComponent.cooldown[hit.target] = HURTCOOLDOWN;
            hit.target->stun_ = true;
            hit.target->hurt_ = true;
            hit.hitter->hit_ = true;
            hit.target->noStickThisUpdate_ = true;

            // if (hurtboxComponent.seedAmount[hit.target-> > 0) {
            //     seedManager.CreateMultipleSeed(
            //         transformComponent.transform[hit.target->.position, 
            //         hurtboxComponent.seedAmount[hit.target->,
            //         hurtboxComponent.seedRadius[hit.target->
            //     );
            // }

            if (!hit.target->GetFlag(EntityS::EF_RecieveKnockback))
                continue;

            vec3 normalizeRes = normalize(hit.collision.resolution);
            hit.target->velocity_ = 
                normalizeRes * hit.hitter->hitbox_.horizontalKb + 
                Transform::worldUp * hit.hitter->hitbox_.verticalKb;

            vec3 planarVelocity = hit.target->velocity_;
            planarVelocity.y = 0.0f;
            if (hit.target->GetFlag(EntityS::EF_HurtFaceForward))
                hit.target->transform_.rotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp); 
            else if (hit.target->GetFlag(EntityS::EF_HurtFaceBack))
                hit.target->transform_.rotation = quatLookAtRH(normalize(-planarVelocity), Transform::worldUp); 
        }

        for (int i = 0; i < 128; i++) {
            if (!entityListS_.Valid(i)) continue;
            entityListS_[i].lastTransform_ = entities_[i].entity.transform_;
            switch(entities_[i].typeId) {
                #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): entities_[i].VAR.Update(); break;
                EXPANDENTITIES
                #undef ENTITYEXP
            }
            entityListS_[i].BaseUpdate();
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
    camera_.Update(entityListS_, inputs_);

    for (int i = 0; i < 128; i++) {
        if (!entityListS_.Valid(i)) continue;
        entityListS_[i].BaseRenderUpdate(timeAccumlulator_ / GlobalTime::TIMESTEP);
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
