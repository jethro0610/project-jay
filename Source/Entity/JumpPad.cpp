#include "JumpPad.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "EntityList.h"
#include "Player.h"
using namespace glm;

EntityDependendies JumpPad::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties JumpPad::GetStaticProperties() {
    return {
        {
            {"p_jump_strength", &jumpStregth_}
        },
        {
            {"p_num_seeds", &numSeeds_},
            {"p_cooldown", &cooldown_},
            {"p_seg_id", &segmentId_},
            {"p_seg_count", &numSegments_},
            {"p_seg_min", &minSegments_},
        },
        {
            {"p_seg_destroy", &destroySeg_},
        }
    };
}

void JumpPad::Init(Entity::InitArgs args) {
    SetFlag(EF_Overlap, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color_terrainclipped");
    materials_[0].transparencyType = TRANSPARENCY_UNORDERED;
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 2.0f;
    overlapbox_.bottom = 2.0f;
    overlapbox_.radius = 2.0f;

    numSeeds_ = 0;
    jumpStregth_ = 100.0f;
    cooldown_ = 0;

    timer_ = 0;

    segmentId_ = -1;
    numSegments_ = 0;
    minSegments_ = 0;
    destroySeg_ = false;
}

void JumpPad::Update() {
    if (timer_ > 0) {
        SetFlag(EF_Homeable, false);
        materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        timer_--;
    }
    else {
        SetFlag(EF_Homeable, true);
        materials_[0].properties.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
    }
}

void JumpPad::OnOverlap(Entity* overlappedEntity) {
    if (timer_ > 0)
        return;
    if (numSegments_ < minSegments_)
        return;

    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = jumpStregth_;
    timer_ = cooldown_;

    if (overlappedEntity->typeId_ == Player::TYPEID)
        ((Player*)overlappedEntity)->EndHoming();

    lastOverlappedEntity_ = overlappedEntity;
    seedManager_->CreateMultipleSeed(transform_.position, numSeeds_, 20.0f, lastOverlappedEntity_, vec3(0.0f, 30.0f, 0.0f));
}

void JumpPad::OnHitlagEnd() {
}

int JumpPad::GetSeeds() {
    return numSeeds_;
}
