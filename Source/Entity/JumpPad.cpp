#include "JumpPad.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Spread/SpreadManager.h"
#include "Seed/SeedManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies JumpPad::GetDeps() {
    return {
        "st_tpillar"
    };
}

EntityProperties JumpPad::GetProperties() {
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
    Entity::Init(args);

    SetFlag(EF_Overlap, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 2.0f;
    pushbox_.bottom = 2.0f;
    pushbox_.radius = 2.0f;

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

    seedManager_->CreateMultipleSeed(transform_.position, numSeeds_, 20.0f, overlappedEntity, vec3(0.0f, 30.0f, 0.0f));
    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = jumpStregth_;
    timer_ = cooldown_;
}
