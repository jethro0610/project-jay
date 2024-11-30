#include "SpeedBoost.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies SpeedBoost::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void SpeedBoost::Init(Entity::InitArgs args) {
    SetFlag(EF_Overlap, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.0f;

    speed_ = 40.0f;
    seed_ = 100;
    cooldown_ = 60 * 10;
    timer_ = 0;
}

void SpeedBoost::Update() {
    if (timer_ > 0)
        timer_--;
}

void SpeedBoost::OnOverlap(Entity* overlappedEntity) {
    if (timer_ > 0)
        return;

    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    Player* player = (Player*)overlappedEntity;

    vec3 planarVelocity = vec3(player->velocity_.x, 0.0f, player->velocity_.z);
    player->Boost(length(planarVelocity), 0.25f);
    seedManager_->CreateMultipleSeed(transform_.position, seed_, 5.0f, player);
    timer_ = 10;
}
