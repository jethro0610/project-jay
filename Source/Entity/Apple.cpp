#include "Apple.h"
#include "Resource/ResourceManager.h"
#include "Tree.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies Apple::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void Apple::Init(Entity::InitArgs args) {
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_Overlap, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color"); 
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(1.0f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.radius = 2.5f;
    overlapbox_.top = 1.25f;
    overlapbox_.bottom = 1.25f;

    active_ = true;
    initialScale_ = transform_.scale;
    growth_ = 1.0f;
    overlappedEntity_ = nullptr;
}

static constexpr float GROWTH_RATE = 0.01f;
void Apple::Update() {
    if (!active_) {
        growth_ += GROWTH_RATE;
        growth_ = min(growth_, 1.0f);
        transform_.scale = glm::mix(vec3(0.0f), initialScale_, growth_);
    }
    else {
        if (!onGround_) {
            velocity_.y -= GRAVITY;
        }
        else {
            velocity_ = vec3(0.0f);
        }
    }
}

static constexpr int HITLAG = 4;
void Apple::OnOverlap(Entity* overlappedEntity) {
    if (
        overlappedEntity_ != nullptr ||
        overlappedEntity->typeId_ == Tree::TYPEID ||
        overlappedEntity->typeId_ == Apple::TYPEID
    ) 
        return;

    overlappedEntity_ = overlappedEntity;
    hitlag_ = HITLAG;
    stun_ = true;
    overlappedEntity->hitlag_ = HITLAG;
}

static constexpr int SEEDS = 500;
void Apple::OnHitlagEnd() {
    seedManager_->CreateMultipleSeed(transform_.position, SEEDS, 24.0f, overlappedEntity_);
    destroy_ = true;
}
