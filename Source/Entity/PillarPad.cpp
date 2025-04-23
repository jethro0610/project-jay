#include "PillarPad.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
using namespace glm;

EntityDependendies PillarPad::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties PillarPad::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_strength", &jumpStrength_}
        },
        {
            // Ints
        },
        {
            // Bools
        }
    };
}

void PillarPad::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color_terrainclipped");
    materials_[0].transparencyType = TRANSPARENCY_UNORDERED;
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.bottom = -0.95f;
    overlapbox_.top = 1.25f;
    overlapbox_.radius = 2.0f;

    hurtbox_.top = 0.5f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.15f;

    SetFlag(EF_Homeable, false);
    SetFlag(EF_Overlap, true);
    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecieveHits, true);

    activated_ = false;
    jumpStrength_ = 100.0f;
}

void PillarPad::OnOverlap(Entity* overlappedEntity) {
    if (!activated_)
        return;

    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = jumpStrength_;

    if (overlappedEntity->typeId_ == Player::TYPEID)
        ((Player*)overlappedEntity)->EndHoming();

    SetFlag(EF_Homeable, false);
    activated_ = false;
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //lastOverlappedEntity_ = overlappedEntity;
    //seedManager_->CreateMultipleSeed(transform_.position, numSeeds_, 20.0f, lastOverlappedEntity_, vec3(0.0f, 30.0f, 0.0f));
}

void PillarPad::OnHurt(HurtArgs args) {
    SetFlag(EF_Homeable, true);
    activated_ = true;
    materials_[0].specularProperties.color = vec4(0.5f, 1.0f, 0.5f, 1.0f);
}

vec3 PillarPad::GetTargetPoint() {
    return transform_.position + vec3(0.0f, -overlapbox_.bottom * transform_.scale.y, 0.0f);
}
