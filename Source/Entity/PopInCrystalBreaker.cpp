#include "PopInCrystalBreaker.h"
#include "Resource/ResourceManager.h"
#include "PopInCrystal.h"
#include "EntityList.h"
#include "Time/Time.h"
#include "Helpers/Shared_Ease.h"
using namespace glm;

EntityDependendies PopInCrystalBreaker::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties PopInCrystalBreaker::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
            {"p_targetid", &targetId_}
        },
        {
            // Bools
        }
    };
}

void PopInCrystalBreaker::Init(Entity::InitArgs args) {
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

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.5f;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_Overlap, true);
    traceDistance_ = 1000.0f;
}

void PopInCrystalBreaker::Start() {
    target_ = (PopInCrystal*)entities_->FindEntityById(targetId_);
    launchPos_ = transform_.position;
}

void PopInCrystalBreaker::Update() {
    launchPos_ += velocity_ * 4.0f * GlobalTime::TIMESTEP;
    if (launched_)
        velocity_.y -= 2.0f;

    const int TRAVEL_TIME = 60;
    if (launched_) {
        timer_++;
        float t = (float)timer_ / TRAVEL_TIME;
        t = EaseInCubic(t);
        t = clamp(t, 0.0f, 0.985f);
        transform_.position = mix(launchPos_, target_->transform_.position, t);
    }
}

void PopInCrystalBreaker::OnHurt(HurtArgs args) {
    launched_ = true;
    SetFlag(EF_StickToGround, false);
    SetFlag(EF_GroundCheck, false);
}

void PopInCrystalBreaker::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity == target_) {
        target_->Break();
        hitlag_ = 16;
        destroy_ = true;
    }
}
