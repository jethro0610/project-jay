#include "SeedBalloon.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
#include "Player.h"
using namespace glm;

EntityDependendies SeedBalloon::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties SeedBalloon::GetStaticProperties() {
    return {
        {
            // Floats
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

void SeedBalloon::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 2.0f;
    overlapbox_.bottom = 2.0f;
    overlapbox_.radius = 1.5f;

    SetFlag(EF_Overlap, true);
    seeds_ = 100;
    lastOverlapped_ = nullptr;
}

void SeedBalloon::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    destroy_ = true;
    overlappedEntity->hitlag_ = 8;
    hitlag_ = 8;
    lastOverlapped_ = overlappedEntity;
}

void SeedBalloon::OnDestroy() {
    seedManager_->CreateMultipleSeed(transform_.position, seeds_, 20.0f, lastOverlapped_);
}

int SeedBalloon::GetSeeds() {
    return seeds_;
}
