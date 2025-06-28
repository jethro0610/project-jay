#include "Ramp.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Helpers/Shared_Ease.h"
using namespace glm;

EntityDependendies Ramp::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Ramp::GetStaticProperties() {
    return {
        {
            {"p_radius", &bubble_->radius},
            {"p_start_height", &bubble_->height},
            {"p_active_height", &activeHeight_},
            {"p_in", &bubble_->inpower},
            {"p_out", &bubble_->outpower}
        },
        {

        },
        {

        }
    };
}

void Ramp::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    SetFlag(EF_Interpolate, true);
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    bubble_ = terrain_->CreateBubble();
    bubble_->position = transform_.position;
    bubble_->active = true;

    bubble_->radius = 30.0f;
    bubble_->inpower = 2.0f;
    bubble_->outpower = 2.0f;
    bubble_->height = 50.0f;

    active_ = false;
    riseTimer_ = 0.0;
}

void Ramp::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}

void Ramp::OnTrigger() {
    active_ = true;
}

void Ramp::PreUpdate() {
    // if (clock_->GetTimeOfDay() >= Clock::Sunrise && clock_->GetTimeOfDay() <= clock_->Sunset)
    //     active_ = true;
    static constexpr int RISE_TIME = 30;
    if (active_ && riseTimer_ < RISE_TIME) {
        riseTimer_++;
        bubble_->height = EaseQuad(((float)riseTimer_ / RISE_TIME)) * activeHeight_;
    }
    bubble_->position = transform_.position;
}

void Ramp::EditorUpdate() {
    bubble_->position = transform_.position;
}
