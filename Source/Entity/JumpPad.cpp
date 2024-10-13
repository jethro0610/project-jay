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
            {"p_cooldown", &cooldown_}
        },
        {

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
    materials_[0].properties.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 2.0f;
    overlapbox_.bottom = 0.0f;
    overlapbox_.radius = 2.0f;

    numSeeds_ = 0;
    jumpStregth_ = 100.0f;
    cooldown_ = 0;

    timer_ = 0;
}

void JumpPad::Update() {
    if (timer_ > 0) {
        SetFlag(EF_Targetable, false);
        materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        timer_--;
    }
    else {
        SetFlag(EF_Targetable, true);
        materials_[0].properties.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
    }
}

void JumpPad::OnOverlap(Entity* overlappedEntity) {
    if (timer_ > 0)
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
