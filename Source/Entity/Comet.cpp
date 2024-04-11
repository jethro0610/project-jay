#include "Comet.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Terrain/Terrain.h"
#include "Seed/SeedManager.h"
#include "Helpers/Random.h"
using namespace glm;

EntityDependendies Comet::GetDeps() {
    return {
        "st_comet"
    };
}

void Comet::Init(Entity::InitArgs args) {
    Entity::Init(args);
    SetFlag(EF_UseVelocity, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_comet");

    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].castShadows = false;
    materials_[0].properties.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materials_[0].numTextures = 0;

    cometTrail_ = args.particleManager.RequestEmitter();
    cometTrail_->properties_.material.shader = resourceManager.GetShader("vs_particle", "fs_particle");
    cometTrail_->properties_.material.castShadows = false;
    cometTrail_->properties_.spawnInterval = 0.025f;
    cometTrail_->properties_.spawnCount = 50;
    cometTrail_->properties_.spawnRadius = 1.0f;
    cometTrail_->properties_.lifetime = 0.5f;
    cometTrail_->properties_.minScale = 0.25f;
    cometTrail_->properties_.maxScale = 1.0f;
    cometTrail_->properties_.endScale = 0.0f;
    cometTrail_->properties_.minVelocity = vec4(-15.0f);
    cometTrail_->properties_.maxVelocity = vec4(15.0f);
    cometTrail_->properties_.startColor = vec4(1.0f, 1.0f, 1.0f, 0.75f);
    cometTrail_->properties_.endColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
    cometTrail_->active_ = true;
    cometTrail_->transform_ = transform_;
    cometTrail_->lastTransform_ = transform_;

    velocity_.x = RandomFloatRange(25.0f, 50.0f);
    velocity_.z = RandomFloatRange(25.0f, 50.0f);
    velocity_.y = -150.0f;
    transform_.rotation = quatLookAt(normalize(velocity_ ), Transform::worldForward);
    transform_.scale = vec3(2.0f);
}

void Comet::Update() {
    if (transform_.position.y < terrain_->GetHeight(transform_.position)) {
        destroy_ = true;
    }
}

void Comet::RenderUpdate() {
    cometTrail_->transform_ = renderTransform_;
}

void Comet::OnDestroy() {
    cometTrail_->release_ = true;
    seedManager_->CreateMultipleSeed(transform_.position, 300, 25.0f); 
}
