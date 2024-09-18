#include "Sleeper.h"
#include "Mower.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Spread/SpreadManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies Sleeper::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void Sleeper::Init(Entity::InitArgs args) {
    SetFlag(EF_RecieveHits, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void Sleeper::OnHurt(HurtArgs args) {
    for (int i = 0; i < 128; i++) {
        Entity* entity = &(*entities_)[i];
        if (entity->typeId_ == Mower::TYPEID) {
            Mower* mower = (Mower*)entity;
            if (mower->target_ == args.attacker) {
                mower->ToggleMode();
            }
        }
    }
}
