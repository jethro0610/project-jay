#include "HeadSpitter.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
#include "GroundSpawnProjectile.h"
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
        GroundSpawnProjectile* projectile = (GroundSpawnProjectile*)&entities_->CreateEntity(GroundSpawnProjectile::TYPEID);

        projectile->spawn_ = TimedHitbox::TYPEID;
        projectile->spawnProperties_.SetFloat("p_horizontalkb", 60);
        projectile->spawnProperties_.SetFloat("p_verticalkb", 20);
        projectile->spawnProperties_.SetInt("p_lifespan", 5);

        projectile->transform_.position = transform_.position + vec3(0.0f, 5.0f, 0.0f);
        projectile->spawnScale_.x = 25.0f;
        projectile->spawnScale_.y = 50.0f;
        projectile->spawnScale_.z = 25.0f;

        projectile->velocity_.y = 20.0f;
        projectile->velocity_.x = 20.0f;

        timer_ = 0;
    }
}
