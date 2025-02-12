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
            {"p_radius", &DYN_MOD_RADIUS((*bubble_))},
            {"p_start_height", &DYN_MOD_VALUE((*bubble_))},
            {"p_active_height", &activeHeight_},
            {"p_in", &DYN_MOD_IN_POWER((*bubble_))},
            {"p_out", &DYN_MOD_OUT_POWER((*bubble_))}
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
    DYN_MOD_POS_X((*bubble_)) = transform_.position.x;
    DYN_MOD_POS_Y((*bubble_)) = transform_.position.z;
    DYN_MOD_SET_ACTIVE((*bubble_), true);

    DYN_MOD_RADIUS((*bubble_)) = 30.0f;
    DYN_MOD_IN_POWER((*bubble_)) = 2.0f;
    DYN_MOD_OUT_POWER((*bubble_)) = 0.5f;
    DYN_MOD_VALUE((*bubble_)) = 0.0f;

    active_ = false;
    activator_.requiredStocks = 2;
    activator_.radius = 32.0f;
    activator_.position = transform_.position + vec3(0.0f, 32.0f, 0.0f);
    riseTimer_ = 0.0;
}

void Ramp::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}

void Ramp::OnActivate() {
    active_ = true;
}

void Ramp::PreUpdate() {
    static constexpr int RISE_TIME = 30;
    if (active_ && riseTimer_ < RISE_TIME) {
        riseTimer_++;
        DYN_MOD_VALUE((*bubble_)) = EaseQuad(((float)riseTimer_ / RISE_TIME)) * activeHeight_;
    }
    DYN_MOD_POS_X((*bubble_)) = transform_.position.x;
    DYN_MOD_POS_Y((*bubble_)) = transform_.position.z;
}

void Ramp::EditorUpdate() {
    DYN_MOD_POS_X((*bubble_)) = renderTransform_.position.x;
    DYN_MOD_POS_Y((*bubble_)) = renderTransform_.position.z;
}
