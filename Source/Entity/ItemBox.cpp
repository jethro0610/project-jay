#include "ItemBox.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies ItemBox::GetDeps() {
    return {
        "st_tpillar"
    };
}

void ItemBox::Init(Entity::InitArgs args) {
    Entity::Init(args);

    SetFlag(EF_RecieveHits, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void ItemBox::OnHurt(HurtArgs args) {
    if (args.attacker->typeId_ == Player::TYPEID) {
        Player* player = (Player*)args.attacker;

        int random = rand() % 2;
        if (random == 0)
            player->item_ = Item::Cut;
        else
            player->item_ = Item::Radius;
    }
}
