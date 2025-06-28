#include "SnakePod.h"
#include "SnakeHead.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies SnakePod::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void SnakePod::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = vec4(1.0f, 1.0f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
    traceDistance_ = 100.0f;

    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_StickToGround, true);
}

void SnakePod::PreUpdate() {
    hitlag_ = head_->hitlag_;
}

void SnakePod::Update() { 
    transform_.position = (prevSegment_->transform_.position + nextSegment_->transform_.position) * 0.5f;
}

void SnakePod::OnHurt(HurtArgs args) {
    // seedManager_->CreateMultipleSeed(transform_.position, Clock::TIME_IN_DAY_SECTION * 0.1f, 16.0f, args.attacker);
    destroy_ = true;
    head_->hitlag_ = args.hitbox->hitlag;
}
