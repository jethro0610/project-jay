#include "LevelClear.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Spread/SpreadManager.h"
#include "Level/Level.h"
#include "Entity/EntityList.h"
using namespace glm;

EntityDependendies LevelClear::GetDeps() {
    return {
        "st_tpillar"
    };
}

void LevelClear::Init(Entity::InitArgs args) {
    Entity::Init(args);

    SetFlag(EF_SendPush, false);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_SendPush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].properties.color = vec4(0.25f, 0.0f, 0.25f, 1.0f);
    materials_[0].castShadows = true;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 0.75f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    canGoNext_ = false;
}

void LevelClear::Update() {
    if (spreadManager_->GetCoverage() >= 0.4f) {
        materials_[0].properties.color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
        canGoNext_ = true;
    }
    else {
        materials_[0].properties.color = vec4(0.25f, 0.0f, 0.25f, 1.0f);
        canGoNext_ = false;
    }

    if (shouldGoNext_) {
        level_->NextPhase();
        spreadManager_->Reset();
        ((Player&)(*entities_)[0]).meter_ = 0.0f;
        shouldGoNext_ = false;
        canGoNext_ = false;
    }
}

void LevelClear::OnHurt(HurtArgs args) {
    if (args.attacker->typeId_ != Player::TYPEID || !canGoNext_)
        return;

    shouldGoNext_ = true;
}
