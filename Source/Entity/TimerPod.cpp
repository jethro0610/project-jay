#include "TimerPod.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies TimerPod::GetDeps() {
    return {
        "st_tpillar"
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
    static constexpr int SEEDS = 500;

    SetFlag(EF_RecieveHits, false);
    seedManager_->CreateMultipleSeed(transform_.position, SEEDS, 8.0f);
    timer_ = 0;
}
