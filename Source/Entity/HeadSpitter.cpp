#include "HeadSpitter.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
#include "TimedHitbox.h"
using namespace glm;

EntityDependendies HeadSpitter::GetDeps() {
    return {
        "st_tpillar"
    };
}

EntityProperties HeadSpitter::GetProperties() {
    return {
        {

        },
        {

        },
        {

        }
    };
}

void HeadSpitter::Init(Entity::InitArgs args) {
    Entity::Init(args);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    timer_ = 0;
}

void HeadSpitter::Update() {
    timer_++;

    if (timer_ >= 30) {
        TimedHitbox* hitbox = (TimedHitbox*)&entities_->CreateEntity(TimedHitbox::TYPEID);
        hitbox->hitbox_.knocback.x = 50.0f;
        hitbox->hitbox_.knocback.y = 100.0f;
        hitbox->transform_.scale.x = 25.0f;
        hitbox->transform_.scale.y = 50.0f;
        hitbox->transform_.scale.z = 25.0f;
        hitbox->lifespan_ = 15;

        hitbox->transform_.position = transform_.position + vec3(10.0f, 0.0f, 0.0f);
        timer_ = 0;
    }
}
