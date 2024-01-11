#include "MultiHitPod.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Resource/ResourceManager.h"
#include "Helpers/Random.h"
#include "Entity/EntityList.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies MultiHitPod::GetDeps() {
    return {
        "st_tpillar"
    };
}

void MultiHitPod::Init(Entity::InitArgs args) {
    Entity::Init(args);

    disableCollision_ = false;
    cooldown_ = 0;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_SendPush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].selectedShader = resourceManager.GetShader("vs_static", "fs_selected");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void MultiHitPod::Update() {
    if (cooldown_ > 0 && hitlag_ == 0)
        cooldown_--;

    if (disableCollision_ && cooldown_ == 0 && !entities_->IsAnyOverlapping(*this))
        disableCollision_ = false;

    if (disableCollision_) {
        SetFlag(EF_RecieveHits, false);
        SetFlag(EF_SendPush, false);
    }
    else {
        SetFlag(EF_RecieveHits, true);
        SetFlag(EF_SendPush, true);
    }
}

void MultiHitPod::OnHurt() {
    seedManager_->CreateMultipleSeed(transform_.position, 200, 20.0f);
    disableCollision_ = true;
    cooldown_ = MAX_COOLDOWN_TICKS;
}
