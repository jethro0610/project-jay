#include "TimedHitbox.h"
#include "Resource/ResourceManager.h"
using namespace glm;

EntityDependendies TimedHitbox::GetDeps() {
    return {
        "st_tpillar"
    };
}

EntityProperties TimedHitbox::GetProperties() {
    return {
        {
            {"p_horizontalkb", &hitbox_.knocback.x},
            {"p_verticalkb", &hitbox_.knocback.y}
        },
        {
            {"p_lifespan", &lifespan_}
        },
        {

        }
    };
}

void TimedHitbox::Init(Entity::InitArgs args) {
    Entity::Init(args);

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
    hitbox_.knocback.x = 20.0f;
    hitbox_.knocback.y = 20.0f;
    hitbox_.diStrength = 1.0f;
    hitbox_.active = true;

    timer_ = 0;
    lifespan_ = 0;
}

void TimedHitbox::Update() {
    timer_++;
    if (timer_ >= lifespan_) {
        destroy_ = true;
    }
}
