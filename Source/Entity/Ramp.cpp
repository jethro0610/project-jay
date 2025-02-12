#include "Ramp.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
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
            {"p_height", &DYN_MOD_VALUE((*bubble_))},
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
    DYN_MOD_VALUE((*bubble_)) = 30.0f;

    activator_.requiredStocks = 2;
    activator_.radius = 32.0f;
    activator_.position = transform_.position + vec3(0.0f, 32.0f, 0.0f);
}

void Ramp::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}

void Ramp::PreUpdate() {
    transform_.position.z += 1.0f;
    DYN_MOD_POS_X((*bubble_)) = transform_.position.x;
    DYN_MOD_POS_Y((*bubble_)) = transform_.position.z;
}

void Ramp::RenderUpdate() {
    // DYN_MOD_POS_X((*bubble_)) = renderTransform_.position.x;
    // DYN_MOD_POS_Y((*bubble_)) = renderTransform_.position.z;
}

void Ramp::EditorUpdate() {
    DYN_MOD_POS_X((*bubble_)) = renderTransform_.position.x;
    DYN_MOD_POS_Y((*bubble_)) = renderTransform_.position.z;
}
