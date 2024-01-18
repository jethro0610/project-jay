#include "Bomb.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
using namespace glm;

EntityDependendies Bomb::GetDeps() {
    return {
        "st_tpillar"
    };
}

void Bomb::Init(Entity::InitArgs args) {
    Entity::Init(args);
    SetFlag(EF_SendPush, true);

    lifeTimer_ = 0;
    riseTimer_ = 0;
    fallTimer_ = 0;

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    // hitbox_.top = 1.0f;
    // hitbox_.bottom = 1.0f;
    // hitbox_.radius = 2.0f;
    // hitbox_.active = true;
    // hitbox_.knocback = vec3(30.0f, 30.0f, 0.0f);

    // TEMP
    transform_.scale = vec3(3.0f);
    transform_.scale.y = 8.0f;

    upHeight_ = transform_.position.y;
    downHeight_ = upHeight_ - DEPTH;
    transform_.position.y = downHeight_;
}

void Bomb::Update() {
    if (riseTimer_ < RISETIME) {
        riseTimer_++;
        float t = 1 - ((float)riseTimer_ / RISETIME);
        t = 1 - (t * t);
        transform_.position.y = std::lerp(downHeight_, upHeight_, t);
    }

    lifeTimer_++; 
    if (lifeTimer_ > LIFETIME) {
        fallTimer_++;
        float t = (float)fallTimer_ / FALLTIME;
        transform_.position.y = std::lerp(upHeight_, downHeight_, t * t);
    }

    if (fallTimer_ > FALLTIME)
        destroy_ = true;
}
