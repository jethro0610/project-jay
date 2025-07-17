#include "Peg.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies Peg::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Peg::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_jump", &jumpStrength_}
        },
        {
            // Ints
            {"p_seeds", &seeds_}
        },
        {
            // Bools
        }
    };
}

void Peg::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(1.0f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 1.25f;
    overlapbox_.bottom = -0.75f;
    overlapbox_.radius = 1.25f;

    SetFlag(EF_Homeable, true);
    SetFlag(EF_Overlap, true);
    SetFlag(EF_SendPush, true);
    seeds_ = 0;
    jumpStrength_ = 100.0f;
}

void Peg::OnDestroy() {
    seedManager_->CreateMultipleSeed(transform_.position, seeds_, 20.0f, lastOverlap_);
}

void Peg::OnOverlap(Entity* overlappedEntity) {
    overlappedEntity->skipGroundCheck_ = true;

    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    lastOverlap_ = overlappedEntity;

    Player* player = (Player*)overlappedEntity;
    player->EndHoming();
    if (player->onGround_)
        return;
    if (player->velocity_.y > 0.0f && player->homingTarget_ == nullptr)
        return;

    player->velocity_.y = jumpStrength_;
    player->hitlag_ = 4;
    hitlag_ = 4;
    destroy_ = true;
}

vec3 Peg::GetTargetPoint() {
    return transform_.position + vec3(0.0f, transform_.scale.y, 0.0f);
}
