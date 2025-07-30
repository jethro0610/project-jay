#include "BigBreaker.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies BigBreaker::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties BigBreaker::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
            {"p_seeds", &seedsOnDestroy_},
            {"p_seedshit", &seedsOnHit_},
            {"p_jump", &jumpStrength_}
        },
        {
            // Bools
        }
    };
}

void BigBreaker::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.75f, 0.75f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 1.25f;
    overlapbox_.bottom = 0.0f;
    overlapbox_.radius = 1.25f;

    hp_ = 2;
    seedsOnDestroy_ = 1000;
    seedsOnHit_ = 500;
    jumpStrength_ = 150;
    timer_ = 0;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_Homeable, true);
}

void BigBreaker::Update() {
    if (timer_ > 0)
        timer_--;
}

void BigBreaker::OnOverlap(Entity* overlappedEntity) {
    if (timer_ > 0)
        return;

    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    timer_ = 20;

    ((Player*)overlappedEntity)->EndHoming();
    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = jumpStrength_;
    lastOverlapped_ = overlappedEntity;

    hitlag_ = 8;
    overlappedEntity->hitlag_ = 8;
    

    hp_--;
    seedManager_->CreateMultipleSeed(transform_.position, seedsOnHit_, 20.0f, lastOverlapped_);
    if (hp_ <= 0)
        destroy_ = true;
}

void BigBreaker::OnDestroy() {
    seedManager_->CreateMultipleSeed(transform_.position, seedsOnDestroy_, 20.0f, lastOverlapped_);
}
