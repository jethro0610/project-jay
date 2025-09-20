#include "Bumper.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityList.h"
#include "Entity/HoleMarker.h"
#include "Player.h"
#include "Time/Time.h"
#include "Helpers/Shared_Ease.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies Bumper::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Bumper::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
        },
        {
            // Bools
            {"p_startpowered", &powered_}
        }
    };
}

void Bumper::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    overlapbox_.top = 1.15f;
    overlapbox_.bottom = 1.15f;
    overlapbox_.radius = 1.15f;
    
    speed_ = 5.0f;
    traceDistance_ = 1.0f;
    lastAttacker_ = nullptr;
    target_ = nullptr;
    travelPos_ = transform_.position;
    travelTimer_ = 0;
    canTarget_ = false;
    powered_ = false;
    fell_ = false;

    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_DownStickOnly, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_CustomKnockback, true);
    SetFlag(EF_Trackable, true);
    SetFlag(EF_RecievePush, true);
    SetFlag(EF_SeedReleaser, true);
}

void Bumper::Start() {
    spawnPos_ = transform_.position;
}

// Last transform needs to be available on overlaps. So PreUpdate
void Bumper::PreUpdate() {
    if (onGround_) {
        velocity_.y -= 6.0f;
        fell_ = false;
        canTarget_ = true;
    }
    else {
        if (overlappingHole_ && !fell_)
            velocity_.y -= 8.0f;
        else
            velocity_.y -= 3.0f;
    }
    traceDistance_ = min(-velocity_.y * GlobalTime::TIMESTEP, 8.0f);

    if (!onGround_ && !overlappingHole_) {
        float height = terrain_->GetRawHeight(transform_.position);
        if (transform_.position.y < height - 10.0f && velocity_.y < 0.0f) {
            vec3 vecToEdge = terrain_->GetDirectionToEdge(transform_.position) * 150.0f; 
            velocity_ = vecToEdge;
            velocity_.y = 150.0f;
        }
    }

    if (timer_ <= 0) {
        if (onGround_) {
            constexpr float FRICTION = 0.05f;
            float speedDecay = 1.0f - FRICTION;
            float acceleration = (speed_ / speedDecay) - speed_; // For when they start moving
            vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
            planarVelocity *= speedDecay;
            velocity_.x = planarVelocity.x;
            velocity_.z = planarVelocity.z;
        }
    }
    else
        timer_--;

    if (!onGround_ && target_ == nullptr && canTarget_ == true && velocity_.y < 0.0f) {
        for (int i = 0; i < EntityList::MAX; i++) {
            Entity& entity = (*entities_)[i];
            if (!entity.alive_)
                continue;
            if (!entity.GetFlag(EF_ProjectileLockable))
                continue;
            vec3 targetPos = entity.GetTarget();
            vec3 directionToTarget = normalize(targetPos - transform_.position);
            float dotDown = dot(directionToTarget, vec3(0.0f, -1.0f, 0.0f)); 
            if (dotDown < 0.25f)
                continue;

            float planarDist2 = distance2(
                vec2(entity.transform_.position.x, entity.transform_.position.z),
                vec2(transform_.position.x, transform_.position.z)
            );
            if (planarDist2 > 300.0f * 300.0f)
                continue;;

            StartTracking(&entity);
            break;
        }
    }

    constexpr int TRAVEL_TIME = 30;
    if (target_ != nullptr) {
        travelTimer_++;

        travelPos_ += velocity_ * GlobalTime::TIMESTEP;
        float t = (float)travelTimer_ / TRAVEL_TIME;
        t = EaseInQuad(t);
        transform_.position = mix(travelPos_, target_->GetTarget(), t);

        if (travelTimer_ >= TRAVEL_TIME)
            StopTracking();
    }
    overlappingHole_ = false;
}

void Bumper::StartTracking(Entity* target) {
    target_ = target;
    travelPos_ = transform_.position;
    SetFlag(EF_UseVelocity, false);
    travelTimer_ = 0;
}

void Bumper::StopTracking() {
    if (target_ == nullptr)
        return;
    target_ = nullptr;
    SetFlag(EF_UseVelocity, true);
    canTarget_ = false;

    vec3 deltaPos = transform_.position - lastTransform_.position;
    velocity_ = (deltaPos / GlobalTime::TIMESTEP) * 0.1f;
}

void Bumper::OnHurt(HurtArgs args) {
    vec3 kb = args.kbVelocity * 1.25f;
    velocity_.x = kb.x;
    velocity_.z = kb.z;
    if (onGround_)
        velocity_.y = -100.0f;
    timer_ = 30;

    if (!onGround_ && args.attacker->velocity_.y > 0.0f)
        velocity_.y = args.attacker->velocity_.y;

    lastAttacker_ = args.attacker;
}

void Bumper::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ == HoleMarker::TYPEID)
        overlappingHole_ = true;

    if (overlappedEntity->GetFlag(EF_ProjectileLockable))
        StopTracking();
}

bool Bumper::OnFallInHole() {
    if (powered_) {
        seedManager_->CreateMultipleSeed(transform_.position, 60, 20.0f, lastAttacker_);
        destroy_ = true;
    }
    else {
        fell_ = true;
        if (velocity_.y <= 0.0f) {
            velocity_ *= 0.5f;
            velocity_.y = 200.0f;
        }
    }
    return powered_;
}

void Bumper::OnCaptureSeed() {
    powered_ = true;
}
