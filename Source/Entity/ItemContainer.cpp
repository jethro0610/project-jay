#include "ItemContainer.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
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
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_StickToGround, true);

    item_ = I_None;
    numItem_ = 1;
}

void ItemContainer::OnHurt(HurtArgs args) {
    if (args.attacker->typeId_ != Player::TYPEID)
        return;

    Player* player = (Player*)args.attacker;
    if (player->item_ != item_) {
        player->item_ = item_;
        player->numItem_ = numItem_;
    }
}
