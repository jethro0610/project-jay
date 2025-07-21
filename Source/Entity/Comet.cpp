#include "Comet.h"
#include "Helpers/EaseType.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Terrain/Terrain.h"
#include "Seed/SeedManager.h"
#include "Helpers/Random.h"
#include "Time/Time.h"
#include "CometSpawner.h"
#include "Helpers/PhysicsHelpers.h"
using namespace glm;

EntityDependendies Comet::GetStaticDependencies() {
    return {
        "st_comet"
    };
}

void Comet::Init(Entity::InitArgs args) {
    SetFlag(EF_UseVelocity, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_comet");

    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    materials_[0].numTextures = 0;

    cometTrail_ = args.particleManager.RequestEmitter();
    cometTrail_->properties_.material.shader = resourceManager.GetShader("vs_particle", "fs_particle");
    cometTrail_->properties_.material.castShadows = false;
    cometTrail_->properties_.spawnInterval = 0.025f;
    cometTrail_->properties_.spawnCount = 100;
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

    bubble_ = terrain_->CreateBubble();
    bubble_->position = transform_.position;
    bubble_->height = -20.0f;
    bubble_->radius = 300.0f;
    bubble_->inpower = 2.0f;
    bubble_->outpower = 2.0f;
    bubble_->active = false;
    fader_ = DynamicFader();
    fader_.activateLength_ = 16.0f;
    fader_.deactivateLength_ = 120.0f;
    fader_.AddModifier(
        bubble_,
        &bubble_->height,
        0.0f,
        -20.0f,
        false,
        E_OutElastic
    );

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 2.0f;
    SetFlag(EF_RecieveHits, true);
    spawner_ = nullptr;
}

void Comet::Start() {
    constexpr float SPAWN_HEIGHT = 1000.0f;
    constexpr float GRAVITY = 1000.0f;

    velocity_.x = RandomFloatRange(25.0f, 50.0f);
    velocity_.z = RandomFloatRange(25.0f, 50.0f);
    velocity_.y = -GRAVITY;

    transform_.position.y = SPAWN_HEIGHT;
    float terrainHeight = terrain_->GetHeight(transform_.position);
    float airtime = (SPAWN_HEIGHT - terrainHeight) / GRAVITY;
    transform_.position.x -= airtime * velocity_.x;
    transform_.position.z -= airtime * velocity_.z;

    transform_.rotation = quatLookAt(normalize(velocity_), Transform::worldForward);
    transform_.scale = vec3(10.0f);
    cometTrail_->properties_.spawnRadius *= 10.0f;
    fader_.DeactivateModifiers(true);
    contact_ = false;
}

void Comet::PreUpdate() {
    bubble_->position = transform_.position;
    fader_.Update();
}

void Comet::Update() {
    float terrainHeight = terrain_->GetHeight(transform_.position);
    if (transform_.position.y <= terrainHeight - velocity_.y * GlobalTime::TIMESTEP && !contact_) {
        velocity_ = vec3(0.0f);
        cometTrail_->active_ = false;
        fader_.StartActivate();
        contact_ = true;
    }
    if (contact_)
        transform_.position.y = terrainHeight;
}

void Comet::RenderUpdate() {
    cometTrail_->transform_ = renderTransform_;
}

void Comet::OnDestroy() {
    cometTrail_->release_ = true;
    terrain_->FreeBubble(bubble_);
}

void Comet::OnHitlagEnd() {
    if (spawner_ != nullptr) {
        spawner_->OnCometDestroyed();
    }
    Sleep();
}

void Comet::OnHurt(HurtArgs args) {
    seedManager_->CreateMultipleSeed(transform_.position, seeds_, 20.0f, args.attacker); 
}

int Comet::GetSeeds() {
    return seeds_;
}
