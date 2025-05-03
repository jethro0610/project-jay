#include "ItemContainer.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "EntityList.h"
#include "ItemToken.h"
#include "Time/Time.h"
#include "Helpers/Random.h"
using namespace glm;

EntityDependendies ItemContainer::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties ItemContainer::GetStaticProperties() {
    return {
        {
        },
        {
            {"p_item", (int*)&item_},
            {"p_count", &numItem_}
        },
        {

        }
    };
}

void ItemContainer::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_StickToGround, true);

    item_ = I_None;
    numItem_ = 1;
}

void ItemContainer::OnHurt(HurtArgs args) {
    if (args.attacker->typeId_ != Player::TYPEID)
        return;

    float time = GlobalTime::GetTime();
    Player* player = (Player*)args.attacker;
    Transform tokenTransform;
    tokenTransform.position = transform_.position;

    for (int i = 0; i < numItem_; i++) {
        ItemToken& itemToken = (ItemToken&)entities_->CreateEntity(ItemToken::TYPEID, tokenTransform);
        itemToken.SetPlayerAndItem(player, item_, 0.2f * i);
    }
}
