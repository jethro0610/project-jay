#include "Game.h"
#include "Time/Time.h"
#include "Collision/Collision.h"
#include <glm/vec3.hpp>
#include <ctime>
using namespace glm;

using namespace std::chrono;

void Game::Init() {
    srand(time(0));
    // Create the camera and assign it to the renderer
    renderer_.camera_ = &camera_;

    camera_.target_ = &entities_[0];

    Entity& player = entities_.CreateEntity(Player::GetTypeID());
    player.transform_.position = vec3(0.0f, 10.0f, 0.0f);

    Entity& rat = entities_.CreateEntity(BumpRat::GetTypeID());
    rat.transform_.position = vec3(10.0f, 10.0f, 10.0f);

    #ifdef _DEBUG
    editor_.StartEditing();
    #endif
}

struct HitS {
    Entity* hitter;
    Entity* target;
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
            if (!entities_[i].alive_) continue;
            if (!entities_[i].GetFlag(Entity::EF_SendHits) && !entities_[i].GetFlag(Entity::EF_RecieveHits))
                continue;
        
            entities_[i].hit_ = false;
            entities_[i].hurt_ = false;
        }

        vector_const<HitS, 128> hitList;
        for (int h = 0; h < 128; h++) {
            if (!entities_[h].alive_) continue;
            if (!entities_[h].GetFlag(Entity::EF_SendHits))
                continue;
            Entity& hitter = entities_[h];

            for (int t = 0; t < 128; t++) {
                if (h == t) continue;
                if (!entities_[t].alive_) continue;
                if (!entities_[t].GetFlag(Entity::EF_RecieveHits)) continue;
                Entity& target = entities_[t];

                Collision collision = Collision::GetCollision(
                    hitter.transform_, 
                    hitter.hitbox_, 
                    target.transform_, 
                    target.hurtbox_
                );
                if (!collision.isColliding)
                    continue;

                vec3 planarForward = hitter.transform_.GetForwardVector();
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

            if (!hit.target->GetFlag(Entity::EF_RecieveKnockback))
                continue;

            vec3 normalizeRes = normalize(hit.collision.resolution);
            hit.target->velocity_ = 
                normalizeRes * hit.hitter->hitbox_.horizontalKb + 
                Transform::worldUp * hit.hitter->hitbox_.verticalKb;

            vec3 planarVelocity = hit.target->velocity_;
            planarVelocity.y = 0.0f;
            if (hit.target->GetFlag(Entity::EF_HurtFaceForward))
                hit.target->transform_.rotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp); 
            else if (hit.target->GetFlag(Entity::EF_HurtFaceBack))
                hit.target->transform_.rotation = quatLookAtRH(normalize(-planarVelocity), Transform::worldUp); 
        }

        for (int i = 0; i < 128; i++) {
            if (!entities_[i].alive_) continue;
            entities_[i].lastTransform_ = entities_[i].transform_;
            switch(rawEntities_[i].entity.typeId_) {
                #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): rawEntities_[i].VAR.Update(); break;
                EXPANDENTITIES
                #undef ENTITYEXP
            }
            entities_[i].BaseUpdate();
        }

        timeAccumlulator_ -= GlobalTime::TIMESTEP;
    }
    camera_.Update(entities_, inputs_);

    for (int i = 0; i < 128; i++) {
        if (!entities_[i].alive_) continue;
        entities_[i].BaseRenderUpdate(timeAccumlulator_ / GlobalTime::TIMESTEP);
        switch(rawEntities_[i].entity.typeId_) {
            #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): rawEntities_[i].VAR.RenderUpdate(); break;
            EXPANDENTITIES
            #undef ENTITYEXP
        }
    }
    particleManager_.Update(GlobalTime::GetDeltaTime());

    renderer_.Render(
        entities_, 
        levelProperties_,
        particleManager_,
        seedManager_,
        spreadManager_, 
        terrain_
    );
}
