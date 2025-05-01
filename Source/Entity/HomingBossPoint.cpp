#include "HomingBossPoint.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
using namespace glm;

EntityDependendies HomingBossPoint::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HomingBossPoint::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
        },
        {
            // Bools
        }
    };
}

void HomingBossPoint::Init(Entity::InitArgs args) {
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

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 2.0f;

    hit_ = false;

    SetFlag(EF_RecieveHits, false);
}

void HomingBossPoint::Activate() {
    SetFlag(EF_RecieveHits, true);
    materials_[0].specularProperties.color = vec4(1.0f, 0.5f, 1.0f, 1.0f);
    hit_ = false;
}

void HomingBossPoint::OnHurt(HurtArgs args) {
    if (args.attacker->typeId_ != Player::TYPEID)
        return;
    
    hit_ = true;
    materials_[0].specularProperties.color = vec4(0.5f, 1.0f, 1.0f, 1.0f);
}
