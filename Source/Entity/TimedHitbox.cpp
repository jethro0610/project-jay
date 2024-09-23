#include "TimedHitbox.h"
#include "Resource/ResourceManager.h"
#include "Spread/SpreadManager.h"
using namespace glm;

EntityDependendies TimedHitbox::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties TimedHitbox::GetStaticProperties() {
    return {
        {
            {"p_horizontalkb", &hitbox_.knocback.x},
            {"p_verticalkb", &hitbox_.knocback.y},
            {"p_spreadradius", &spreadRadius_}
        },
        {
            {"p_lifespan", &lifespan_},
        },
        {
        }
    };
}

void TimedHitbox::Init(Entity::InitArgs args) {
    SetFlag(EF_SendHits, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(1.0f, 0.5f, 0.5f, 1.0f);

    hitbox_.top = 1.0f;
    hitbox_.bottom = 1.0f;
    hitbox_.radius = 1.0f;
    hitbox_.diStrength = 1.0f;
    hitbox_.active = true;
    hitbox_.forwardRange = -1.0f;

    timer_ = 0;
    lifespan_ = 0;
}

void TimedHitbox::Update() {
    timer_++;
    if (timer_ >= lifespan_)
        destroy_ = true;

    int intSpreadRadius = (spreadRadius_ * max(transform_.scale.x, transform_.scale.z)) / SpreadManager::SPREAD_DIST;
    spreadManager_->RemoveSpread(transform_.position, intSpreadRadius);
}
