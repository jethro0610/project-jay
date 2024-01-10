#include "MagicSeed.h"
#include "Seed/SeedManager.h"
#include "Resource/ResourceManager.h"

EntityDependendies MagicSeed::GetDeps() {
    return {
        "st_tpillar"
    };
}

void MagicSeed::Init(Entity::InitArgs args) {
    Entity::Init(args);

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_SendPush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].selectedShader = resourceManager.GetShader("vs_static", "fs_selected");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.0f;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void MagicSeed::OnHurt() {
    seedManager_->CreateMultipleSeed(transform_.position, 200, 20.0f);
}
