#include "Game.h"
#include "Time/Time.h"
#include "Collision/Collision.h"
#include "Logging/Logger.h"
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ctime>
#include "Rendering/PC_VertexTypes.h"
using namespace glm;

using namespace std::chrono;

#define ENTITYEXP(TYPE, VAR, ID) Entity::TypeID TYPE::TYPEID = ID;
EXPANDENTITIES
#undef ENTITYEXP

void Game::Init() {
    srand(time(0));
    renderer_.camera_ = &camera_;
    camera_.target_ = &entities_[0];

    #ifdef _DEBUG
    editor_.StartEditing();
    #endif

    float lastDeltaTime = 0.0f;
    while (running_) {
        GlobalTime::UpdateTime();
        SCREENLINE(0, "Frame Time(ms): " + std::to_string(GlobalTime::GetDeltaTime() * 1000.0f));

        platform_.FlushMouse();
        glfwPollEvents();
        platform_.UpdateMouseMovement();
        platform_.PollGamepad();
        UpdateInputs_P();

        #ifdef _DEBUG
        if (!editor_.IsActive())
            Update();
        else
            editor_.Update();
        #else
            Update(); 
        #endif
    }

    bgfx::shutdown();
}

struct Hit {
    Entity* hitter;
    Entity* target;
    Collision collision;
};

struct Push {
    Entity* a;
    bool sendA;
    bool recieveA;

    Entity* b;
    bool sendB;
    bool recieveB;
    Collision collision;
};

struct Overlap {
    Entity* a;
    Entity* b;
    Collision collision;
};

void Game::Update() {
    SCREENLINE(8, std::to_string(spreadManager_.GetCoverage()));
    if (spreadManager_.GetCoverage() > 0.33f) {
        editor_.StartEditing();
        return;
    }

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
            entities_[i].lastTransform_ = entities_[i].transform_;
        }

        vector_const<Overlap, 128> overlapList;
        for (int a = 0; a < 128; a++) {
            if (!entities_[a].alive_) continue;
            bool overlapA = entities_[a].GetFlag(Entity::EF_Overlap);

            for (int b = a + 1; b < 128; b++) {
                if (a == b)
                    continue;

                if (!entities_[b].alive_) continue;
                if (!overlapA && !entities_[b].GetFlag(Entity::EF_Overlap)) continue;

                Collision collision = Collision::GetCollision(
                    entities_[a].transform_,
                    entities_[a].pushbox_,
                    entities_[b].transform_,
                    entities_[b].pushbox_
                );

                if (collision.isColliding) {
                    overlapList.push_back({&entities_[a], &entities_[b], collision});
                }
            }
        }

        for (Overlap& overlap : overlapList) {
            switch(overlap.a->typeId_) {
                #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)overlap.a)->OnOverlap(overlap.b); break;
                EXPANDENTITIES
                #undef ENTITYEXP
            }

            switch(overlap.b->typeId_) {
                #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)overlap.b)->OnOverlap(overlap.a); break;
                EXPANDENTITIES
                #undef ENTITYEXP
            }
        }

        vector_const<Hit, 128> hitList;
        for (int h = 0; h < 128; h++) {
            if (!entities_[h].alive_) continue;
            if (!entities_[h].GetFlag(Entity::EF_SendHits)) continue;
            if (!entities_[h].hitbox_.active) continue;
            if (entities_[h].hitlag_ > 0) continue;
            if (entities_[h].stun_) continue;
            Entity& hitter = entities_[h];

            for (int t = 0; t < 128; t++) {
                if (h == t) continue;
                if (!entities_[t].alive_) continue;
                if (!entities_[t].GetFlag(Entity::EF_RecieveHits)) continue;
                if (entities_[t].hitlag_ > 0) continue;
                if (entities_[t].hurtCooldown_ > 0) continue;
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

        for (Hit& hit : hitList) {
            hit.target->stun_ = true;
            hit.target->hurtCooldown_ = 30;
            hit.target->skipGroundCheck_ = true;
            hit.target->onGround_ = false;
            hit.target->initHitlag_ = true;
            hit.target->hitlag_ = hit.hitter->hitbox_.hitlag;
            hit.hitter->hitlag_ = hit.hitter->hitbox_.hitlag;
            hit.hitter->initHitlag_ = true;

            switch(hit.target->typeId_) {
                #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)hit.target)->OnHurt({hit.hitter}); break;
                EXPANDENTITIES
                #undef ENTITYEXP
            }

            switch(hit.hitter->typeId_) {
                #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)hit.hitter)->OnHit({hit.target}); break;
                EXPANDENTITIES
                #undef ENTITYEXP
            }

            if (!hit.target->GetFlag(Entity::EF_RecieveKnockback))
                continue;

            vec3 normalizeRes = normalize(hit.collision.resolution);
            vec3 planarKB = vec3(hit.hitter->hitbox_.knocback.x, 0.0f, hit.hitter->hitbox_.knocback.z);
            float kbLength = length(planarKB);

            quat kbQuat = quatLookAtRH(planarKB / kbLength, Transform::worldUp);
            quat resQuat = quatLookAtRH(normalizeRes, Transform::worldUp);
            quat finalKBQuat = slerp(kbQuat, resQuat, hit.hitter->hitbox_.diStrength);
            
            hit.target->velocity_ = finalKBQuat * Transform::worldForward * kbLength;
            hit.target->velocity_.y = hit.hitter->hitbox_.knocback.y;

            vec3 planarVelocity = hit.target->velocity_;
            planarVelocity.y = 0.0f;
            if (hit.target->GetFlag(Entity::EF_HurtFaceForward))
                hit.target->transform_.rotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp); 
            else if (hit.target->GetFlag(Entity::EF_HurtFaceBack))
                hit.target->transform_.rotation = quatLookAtRH(normalize(-planarVelocity), Transform::worldUp); 
        }

        vector_const<Push, 128> pushList;
        for (int a = 0; a < 128; a++) {
            if (!entities_[a].alive_) continue;
            bool sendA = entities_[a].GetFlag(Entity::EF_SendPush);
            bool recieveA = entities_[a].GetFlag(Entity::EF_RecievePush);
            if (!sendA && !recieveA) continue;
            Entity& entityA = entities_[a];

            for (int b = a + 1; b < 128; b++) {
                if (a == b) continue;
                if (!entities_[b].alive_) continue;
                bool sendB = entities_[b].GetFlag(Entity::EF_SendPush);
                bool recieveB = entities_[b].GetFlag(Entity::EF_RecievePush);
                if (!sendB && !recieveB) continue;
                Entity& entityB = entities_[b];

                Collision collision = Collision::GetCollision(
                    entityA.transform_, 
                    entityA.pushbox_, 
                    entityB.transform_, 
                    entityB.pushbox_
                );

                if (collision.isColliding) {
                    pushList.push_back({&entityA, sendA, recieveA, &entityB, sendB, recieveB, collision});
                }
            }
        }

        for (Push& push : pushList) {
            if (push.sendA && push.recieveA && push.sendB && push.recieveB) {
                push.a->transform_.position -= push.collision.resolution * 0.5f;
                push.b->transform_.position += push.collision.resolution * 0.5f;

                switch(push.a->typeId_) {
                    #define ENTITYEXP(TYPE, VAR, ID) \
                    case ID: ((TYPE*)push.a)->OnPush(-push.collision.resolution * 0.5f); break;
                    EXPANDENTITIES
                    #undef ENTITYEXP
                }

                switch(push.b->typeId_) {
                    #define ENTITYEXP(TYPE, VAR, ID) \
                    case ID: ((TYPE*)push.b)->OnPush(push.collision.resolution * 0.5f); break;
                    EXPANDENTITIES
                    #undef ENTITYEXP
                }
            }
            else if (push.sendA && push.recieveB) {
                push.b->transform_.position += push.collision.resolution * 1.0f;
                switch(push.b->typeId_) {
                    #define ENTITYEXP(TYPE, VAR, ID) \
                    case ID: ((TYPE*)push.b)->OnPush(push.collision.resolution * 1.0f); break;
                    EXPANDENTITIES
                    #undef ENTITYEXP
                }
            }
            else if (push.sendB && push.recieveA) {
                push.a->transform_.position -= push.collision.resolution * 1.0f;
                switch(push.a->typeId_) {
                    #define ENTITYEXP(TYPE, VAR, ID) \
                    case ID: ((TYPE*)push.a)->OnPush(-push.collision.resolution * 1.0f); break;
                    EXPANDENTITIES
                    #undef ENTITYEXP
                }
            }
        }

        for (int i = 0; i < 128; i++) {
            if (!entities_[i].alive_) continue;
            if (entities_[i].hitlag_ == 0) {
                switch(rawEntities_[i].entity.typeId_) {
                    #define ENTITYEXP(TYPE, VAR, ID) case ID: rawEntities_[i].VAR.Update(); break;
                    EXPANDENTITIES
                    #undef ENTITYEXP
                }
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
            #define ENTITYEXP(TYPE, VAR, ID) case ID: rawEntities_[i].VAR.RenderUpdate(); break;
            EXPANDENTITIES
            #undef ENTITYEXP
        }
    }
    seedManager_.GetCaptures(entities_);
    seedManager_.CalculatePositions(terrain_);
    particleManager_.Update(GlobalTime::GetDeltaTime());
    entities_.DestroyFlaggedEntities();

    renderer_.Render(
        entities_, 
        level_.properties_,
        particleManager_,
        seedManager_,
        spreadManager_, 
        terrain_
    );
}
