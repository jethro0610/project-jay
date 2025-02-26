#include "Pinata.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
#include "Game/Clock.h"
using namespace glm;

EntityDependendies Pinata::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Pinata::GetStaticProperties() {
    return {
        {},
        {{"p_hits", &hitsTillBreak_}},
        {}
    };
}

void Pinata::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = materials_[0].specularProperties.Default();
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hitbox_.radius = 2.0f;
    hitbox_.top = 1.0f;
    hitbox_.bottom = 2.0f;

    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);

    hitsTillBreak_ = 3;
    hits_ = 0;
}

void Pinata::Update() {
    if (!onGround_)
        velocity_.y -= 2.0f;
    else {
        velocity_.y = 0.0f;
        vec3 planarVelocity = velocity_;
        planarVelocity.y = 0.0;
        planarVelocity *= 0.9f;
        velocity_.x = planarVelocity.x;
        velocity_.z = planarVelocity.z;
    }

}

void Pinata::OnHurt(HurtArgs args) {
    hits_++;

    if (hits_ >= hitsTillBreak_) {
        seedManager_->CreateMultipleSeed(
            transform_.position, 
            Clock::TIME_IN_DAY_SECTION * 0.15, 
            128.0f, 
            args.attacker, 
            velocity_ * 0.25f,
            1.0f
        );
        destroy_ = true;
    }
    else
        seedManager_->CreateMultipleSeed(
            transform_.position, 
            Clock::TIME_IN_DAY_SECTION * 0.05, 
            32.0f, 
            args.attacker, 
            velocity_ * 0.25f,
            1.0f
        );
}
