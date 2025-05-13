#include "HomingBossProjectile.h"
#include "Resource/ResourceManager.h"
#include "Helpers/Shared_Ease.h"
#include "HomingBoss.h"
#include "EntityList.h"
#include "Time/Time.h"
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

    timer_ = 0;
    launched_ = false;
    spawnTransform_ = transform_;
    canWake_ = false;
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveKnockback, true);
}

void HomingBossProjectile::Start() {
    target_ = (HomingBoss*)entities_->FindEntityByType(HomingBoss::TYPEID);
}

void HomingBossProjectile::Update() {
    const int TRAVEL_TIME = 90;
    if (launched_) {
        launchPoint_ += velocity_ * GlobalTime::TIMESTEP * 4.0f;
        timer_++;
        float t = (float)timer_ / TRAVEL_TIME;
        t = EaseInQuad(t);
        transform_.position = mix(launchPoint_, target_->transform_.position, t);
    }

    if (timer_ >= TRAVEL_TIME) {
        launched_ = false;
        timer_ = 0;
    }
}

void HomingBossProjectile::SleepUpdate() {
    if (!canWake_)
        return;

    const int WAKE_TIME = 30 * 60;
    wakeTimer_++;
    if (wakeTimer_ >= WAKE_TIME) {
        Wake();
        wakeTimer_ = 0;
    }
}

void HomingBossProjectile::OnHurt(HurtArgs args) {
    launched_ = true;
    launchPoint_ = transform_.position;
    velocity_.y += 50.0f;
}

void HomingBossProjectile::Reset() {
    timer_ = 0;
    launched_ = false;
    transform_ = spawnTransform_;
}
