#include "TimerPod.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies TimerPod::GetDeps() {
    return {
        "st_tpillar"
    };
}

EntityProperties TimerPod::GetProperties() {
    return {
        {

        },
        {
            {"p_seeds", &seedAmount_}
        },
        {

        }
    };
}

void TimerPod::Init(Entity::InitArgs args) {
    Entity::Init(args);

    SetFlag(EF_RecieveHits, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    timer_ = 0;
    seedAmount_ = 500;
}

void TimerPod::Update() {
    static constexpr int REACTIVE_TIME = 60 * 10;

    if (timer_ < REACTIVE_TIME)
        timer_++;
    else if(timer_ == REACTIVE_TIME) {
        timer_++;
        SetFlag(EF_RecieveHits, true);
    }
}

void TimerPod::OnHurt(HurtArgs args) {
    SetFlag(EF_RecieveHits, false);
    seedManager_->CreateMultipleSeed(transform_.position, seedAmount_, 16.0f, args.attacker);
    timer_ = 0;
}
