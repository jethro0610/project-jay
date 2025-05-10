#include "HomingBossAttack.h"
#include "HomingBoss.h"
#include "ItemToken.h"
#include "EntityList.h"
#include "Resource/ResourceManager.h"
using namespace glm;

EntityDependendies HomingBossAttack::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HomingBossAttack::GetStaticProperties() {
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

void HomingBossAttack::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 2.0f;

    homingBoss_ = nullptr;
    player_ = nullptr;
    active_ = false;
    hide_ = true;
}

void HomingBossAttack::Activate() {
    SetFlag(EF_RecieveHits, true);
    materials_[0].specularProperties.color = vec4(0.85f, 0.85f, 0.85f, 1.0f);
    active_ = true;
    hide_ = false;
}

void HomingBossAttack::Deactivate() {
    SetFlag(EF_RecieveHits, false);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    active_ = false;
    hide_ = true;
}

void HomingBossAttack::OnHurt(HurtArgs args) {
    homingBoss_->OnAttackHit();

    Transform tokenTransform;
    tokenTransform.position = transform_.position;
    ItemToken& itemToken = (ItemToken&)(entities_->CreateEntity(ItemToken::TYPEID, tokenTransform));
    itemToken.SetPlayerAndItem(player_, Item::I_Homing);
    Deactivate();
}
