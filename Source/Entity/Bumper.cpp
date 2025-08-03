#include "Bumper.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityList.h"
#include "Entity/HoleMarker.h"
#include "Player.h"
#include "Time/Time.h"
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
            {"p_friction", &friction_},
            {"p_gravity", &gravity_}
        },
        {
        },
        {
            // Bools
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
    
    friction_ = 0.05f;
    speed_ = 5.0f;
    traceDistance_ = 1.0f;
    gravity_ = 2.0f; 
    holeMarker_ = nullptr;

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
}

void Bumper::Start() {
    spawnPos_ = transform_.position;
    player_ = entities_->FindEntityByType(Player::TYPEID);
}

void Bumper::Update() {
    if (onGround_)
        velocity_.y -= 6.0f;
    else {
        velocity_.y -= gravity_;
        timer_ = 0;
    }
    traceDistance_ = min(-velocity_.y * GlobalTime::TIMESTEP, 8.0f);

    if (timer_ <= 0) {
        if (onGround_) {
            float speedDecay = 1.0f - friction_;
            float acceleration = (speed_ / speedDecay) - speed_; // For when they start moving
            vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
            planarVelocity *= speedDecay;
            velocity_.x = planarVelocity.x;
            velocity_.z = planarVelocity.z;
        }
    }
    else
        timer_--;

    if (!onGround_) {
        for (int i = 0; i < EntityList::MAX; i++) {
            Entity& entity = (*entities_)[i];
            if (!entity.alive_)
                continue;
            if (!entity.GetFlag(EF_ProjectileLockable))
                continue;
            if (transform_.position.y < entity.transform_.position.y)
                continue;
        }
    }
}

void Bumper::OnHurt(HurtArgs args) {
    velocity_ = args.kbVelocity * 1.15f;
    velocity_.y = 0.0f;
    timer_ = 30;
}
