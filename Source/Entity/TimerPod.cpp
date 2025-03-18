#include "TimerPod.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
#include "Player.h"
#include "Time/Time.h"
using namespace glm;

EntityDependendies TimerPod::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties TimerPod::GetStaticProperties() {
    return {
        {

        },
        {
            {"p_seeds", &seedAmount_},
            {"p_cooldown", &cooldown_}
        },
        {
            {"p_hitonly", &hitOnly_}
        }
    };
}

void TimerPod::Init(Entity::InitArgs args) {
    hitOnly_ = true;
    if (hitOnly_)
        SetFlag(EF_RecieveHits, true);
    else
        SetFlag(EF_Overlap, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.5f;

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.5f;

    timer_ = 0;
    seedAmount_ = 500;
    cooldown_ = 60;
}

void TimerPod::Update() {
    if (timer_ < cooldown_)
        timer_++;
    else if(timer_ == cooldown_) {
        timer_++;
        if (hitOnly_)
            SetFlag(EF_RecieveHits, true);
    }
}

void TimerPod::OnHurt(HurtArgs args) {
    SetFlag(EF_RecieveHits, false);
    seedManager_->CreateMultipleSeed(transform_.position, seedAmount_, 32.0f, args.attacker);
    timer_ = 0;
}

void TimerPod::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ == Player::TYPEID && timer_ >= cooldown_) {
        seedManager_->CreateMultipleSeed(transform_.position, seedAmount_, 16.0f, overlappedEntity);
        timer_ = 0;
    }
}
