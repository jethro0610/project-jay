#include "GroundSpawnProjectile.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies GroundSpawnProjectile::GetDeps() {
    return {
        "st_tpillar"
    };
}

EntityProperties GroundSpawnProjectile::GetProperties() {
    return {
        {

        },
        {

        },
        {

        }
    };
}

void GroundSpawnProjectile::Init(Entity::InitArgs args) {
    Entity::Init(args);

    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_GroundCheck, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    gravity_ = 1.0f;
    spawn_ = -1;
    spawnScale_ = vec3(1.0f);
    spawnProperties_ = EntityPropertiesAssigner();
}

void GroundSpawnProjectile::Update() {
    velocity_.y -= gravity_; 
    if (onGround_)
        destroy_ = true;
}

void GroundSpawnProjectile::OnDestroy() {
    Entity* spawnedEntity = &entities_->CreateEntity(spawn_); 
    EntityProperties properties;

    spawnedEntity->AssignProperties(spawnProperties_);
    spawnedEntity->transform_.position = transform_.position;
    spawnedEntity->transform_.scale = spawnScale_;
}
