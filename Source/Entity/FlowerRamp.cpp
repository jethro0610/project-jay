#include "FlowerRamp.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
using namespace glm;

EntityDependendies FlowerRamp::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties FlowerRamp::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_in", &bubble_->inpower},
            {"p_out", &bubble_->outpower},
            {"p_height", &bubble_->height},
            {"p_radius", &bubble_->radius}
        },
        {
            // Ints
            {"p_maxwet", &maxWet_}
        },
        {
            // Bools
            {"p_active", &bubble_->active}
        }
    };
}

void FlowerRamp::Init(Entity::InitArgs args) {
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

    bubble_ = terrain_->CreateBubble();
    bubble_->active = true;
    bubble_->inpower = 2.0f;
    bubble_->outpower = 2.0f;
    bubble_->height = 100.0f;
    bubble_->radius = 100.0f;

    maxWet_ = 1;
    numWet_ = 0;

    fader_ = DynamicFader();
    fader_.activateLength_ = 30.0f;

    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_Interpolate, true);
    traceDistance_ = 1000.0f;
}

void FlowerRamp::Start() {
    fader_.AddModifier(bubble_, &bubble_->height, 0.0f, bubble_->height, false, EaseType::E_OutElastic);
    if (bubble_->active)
        fader_.ActivateModifiers(true);
    else
        fader_.DeactivateModifiers(true);
}

void FlowerRamp::Update() {
    fader_.Update();
    bubble_->position = transform_.position;
}

void FlowerRamp::EditorUpdate() {
    bubble_->position = transform_.position;
}

void FlowerRamp::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}

void FlowerRamp::OnWet() {
    numWet_++;
    if (numWet_ >= maxWet_)
        fader_.StartActivate();
}
