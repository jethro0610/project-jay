#include "Game.h"
#include "Time/Time.h"
#include "Collision/Collision.h"
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ctime>
#include "Rendering/PC_VertexTypes.h"
using namespace glm;

using namespace std::chrono;

#define ENTITYEXP(TYPE, VAR, ID) TypeID TYPE::TYPEID = ID;
EXPANDENTITIES
#undef ENTITYEXP

#ifdef _DEBUG
#define CAMERA_INPUTS cameraInputs_
#else
#define CAMERA_INPUTS inputs_
#endif

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
    Hitbox* hitbox;
    Collision collision;
    Hitbox::HitType hitType;
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
    // if (spreadManager_.GetCoverage() > 0.33f) {
    //     editor_.StartEditing();
    //     return;
    // }

    timeAccumlulator_ += GlobalTime::GetDeltaTime();
    #ifdef _DEBUG
    cameraInputs_ = inputs_;
    #endif
    while (timeAccumlulator_ >= GlobalTime::TIMESTEP) {
        #ifdef _DEBUG
        if (editor_.IsActive()) {
            timeAccumlulator_ = 0.0f;
            return;
        }
        if (platform_.pressedKeys_['R']) {
            if (camera_.target_ != nullptr)
                camera_.target_ = nullptr;
            else
                camera_.target_ = &entities_[0];
        }
        if (platform_.pressedKeys_['T'] && camera_.target_ == nullptr)
            entities_[0].transform_.position = camera_.GetLookPosition();
        if (camera_.target_ == nullptr)
            inputs_ = {};
        if (platform_.pressedKeys_['P'] || platform_.gamepad_.pressedButtons_[Gamepad::LTHUMB])
            rawEntities_[0].player.Boost(30.0f, 1.0f);
        if (platform_.pressedKeys_['-'])
            clock_.AddTime(-Clock::TIME_IN_DAY_SECTION);
        if (platform_.pressedKeys_['='])
            clock_.AddTime(Clock::TIME_IN_DAY_SECTION);
        if (platform_.pressedKeys_['I']) {
            entities_[0].transform_.position.y = terrain_.GetHeight(entities_[0].transform_.position);
            entities_[0].velocity_.y = 0.0f;
        }
        #endif

        FlushInputs_P();
        #ifdef _DEBUG
        // Prevent updates if the editor started editing here
        if (editor_.IsActive())
            continue;
        #endif

        for (int i = 0; i < EntityList::MAX; i++) {
            if (!entities_[i].alive_) continue;
            entities_[i].lastTransform_ = entities_[i].transform_;
        }

        spreadManager_.ClearTramples();

        for (int i = 0; i < EntityList::MAX; i++) {
            if (!entities_[i].alive_) continue;
            rawEntities_[i].entity.DoPreUpdate();
        }

        vector_const<Overlap, EntityList::MAX> overlapList;
        for (int a = 0; a < EntityList::MAX; a++) {
            if (!entities_[a].alive_) continue;
            if (entities_[a].hitlag_ > 0) continue;
            bool overlapA = entities_[a].GetFlag(Entity::EF_Overlap);

            for (int b = a + 1; b < EntityList::MAX; b++) {
                if (a == b)
                    continue;

                if (!entities_[b].alive_) continue;
                if (entities_[b].hitlag_ > 0) continue;
                if (!overlapA && !entities_[b].GetFlag(Entity::EF_Overlap)) continue;

                Collision collision = Collision::GetCollision(
                    entities_[a].transform_,
                    entities_[a].overlapbox_,
                    entities_[b].transform_,
                    entities_[b].overlapbox_
                );

                if (collision.isColliding) {
                    overlapList.push_back({&entities_[a], &entities_[b], collision});
                }
            }
        }

        for (Overlap& overlap : overlapList) {
            overlap.a->DoOverlap(overlap.b);
            overlap.b->DoOverlap(overlap.a);
        }

        vector_const<Hit, EntityList::MAX> hitList;
        for (int h = 0; h < EntityList::MAX; h++) {
            if (!entities_[h].alive_) continue;
            if (!entities_[h].GetFlag(Entity::EF_SendHits)) continue;
            if (!entities_[h].hitbox_.active) continue;
            if (entities_[h].hitlag_ > 0) continue;
            if (entities_[h].stun_) continue;
            Entity& hitter = entities_[h];

            for (int t = 0; t < EntityList::MAX; t++) {
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
                    hitList.push_back({&hitter, &target, &hitter.hitbox_, collision, hitter.hitbox_.hitType});
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

            vec3 kbVelocity = vec3(0.0f);
            quat kbRotation;
            if (hit.target->GetFlag(Entity::EF_RecieveKnockback)) {
                vec3 normalizeRes = normalize(hit.collision.resolution);
                vec3 planarKB = vec3(hit.hitter->hitbox_.knocback.x, 0.0f, hit.hitter->hitbox_.knocback.z);
                float kbLength = length(planarKB);

                quat kbQuat = quatLookAtRH(planarKB / kbLength, Transform::worldUp);
                quat resQuat = quatLookAtRH(normalizeRes, Transform::worldUp);
                quat finalKBQuat = slerp(kbQuat, resQuat, hit.hitter->hitbox_.diStrength);
                
                kbVelocity = finalKBQuat * Transform::worldForward * kbLength;
                kbVelocity.y = hit.hitter->hitbox_.knocback.y;
                if (!hit.target->GetFlag(Entity::EF_CustomKnockback))
                    hit.target->velocity_ = kbVelocity;

                vec3 planarVelocity = kbVelocity;
                planarVelocity.y = 0.0f;
                if (hit.target->GetFlag(Entity::EF_HurtFaceForward))
                    kbRotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp); 
                else if (hit.target->GetFlag(Entity::EF_HurtFaceBack))
                    kbRotation = quatLookAtRH(normalize(-planarVelocity), Transform::worldUp); 

                if (!hit.target->GetFlag(Entity::EF_CustomKnockback))
                    hit.target->transform_.rotation = kbRotation;
            }

            hit.target->DoHurt({hit.hitter, hit.hitbox, hit.hitType, kbVelocity, kbRotation});
            hit.hitter->DoHit({hit.target, hit.hitType});
        }

        vector_const<Push, EntityList::MAX> pushList;
        for (int a = 0; a < EntityList::MAX; a++) {
            if (!entities_[a].alive_) continue;
            bool sendA = entities_[a].GetFlag(Entity::EF_SendPush);
            bool recieveA = entities_[a].GetFlag(Entity::EF_RecievePush);
            if (!sendA && !recieveA) continue;
            Entity& entityA = entities_[a];

            for (int b = a + 1; b < EntityList::MAX; b++) {
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

                push.a->DoPush(-push.collision.resolution * 0.5f);
                push.b->DoPush(push.collision.resolution * 0.5f);
            }
            else if (push.sendA && push.recieveB) {
                push.b->transform_.position += push.collision.resolution * 1.0f;
                push.b->DoPush(push.collision.resolution * 1.0f);
            }
            else if (push.sendB && push.recieveA) {
                push.a->transform_.position -= push.collision.resolution * 1.0f;
                push.a->DoPush(-push.collision.resolution * 1.0f);
            }
        }

        for (int i = 0; i < EntityList::MAX; i++) {
            if (!entities_[i].alive_) continue;
            if (entities_[i].hitlag_ == 0) {
                rawEntities_[i].entity.DoUpdate();
            }
            entities_[i].BaseUpdate();
        }

        for (int i = 0; i < EntityList::MAX; i++) {
            if (!entities_[i].sleep_) continue;
            rawEntities_[i].entity.DoSleepUpdate();
        }

        timeAccumlulator_ -= GlobalTime::TIMESTEP;
    }
    camera_.Update(entities_, CAMERA_INPUTS);

    for (int i = 0; i < EntityList::MAX; i++) {
        if (!entities_[i].alive_) continue;
        entities_[i].BaseRenderUpdate(timeAccumlulator_ / GlobalTime::TIMESTEP);
        entities_[i].DoRenderUpdate();

        if (entities_[i].GetFlag(Entity::EF_UseTrail))
            entities_[i].RecordTrail();
    }
    seedManager_.GetCaptures(entities_);
    seedManager_.CalculatePositions(terrain_);
    particleManager_.Update(GlobalTime::GetDeltaTime());
    entities_.DestroyFlaggedEntities();

    renderer_.Render(
        clock_,
        entities_, 
        level_.properties_,
        particleManager_,
        seedManager_,
        spreadManager_, 
        terrain_
    );
}
