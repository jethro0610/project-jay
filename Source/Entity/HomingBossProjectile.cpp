#include "HomingBossProjectile.h"
#include "Resource/ResourceManager.h"
#include "ItemToken.h"
#include "Player.h"
#include "EntityList.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies HomingBossProjectile::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HomingBossProjectile::GetStaticProperties() {
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

void HomingBossProjectile::Init(Entity::InitArgs args) {
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

    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
}

void HomingBossProjectile::Update() {
    velocity_.y -= GRAVITY;
    if (onGround_) {
        velocity_.x = 0.0f;
        velocity_.z = 0.0f;
        transform_.scale.y = 64.0f;
    }
}

void HomingBossProjectile::OnHurt(HurtArgs args) {
    if (args.attacker->typeId_ != Player::TYPEID)
        return;
    Player* player = (Player*)args.attacker;

    destroy_ = true; // Maybe don't destroy and just hide?
    Transform tokenTransform;
    tokenTransform.position = transform_.position;
    for (int i = 0; i < 2; i++) {
        ItemToken& token = (ItemToken&)entities_->CreateEntity(ItemToken::TYPEID, tokenTransform);
        token.SetPlayerAndItem(player, Item::I_Homing, i * 0.25f);
    }
}
