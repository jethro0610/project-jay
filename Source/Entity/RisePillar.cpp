#include "RisePillar.h"
#include "Resource/ResourceManager.h"
#include "Time/Time.h"
#include "Helpers/Shared_Ease.h"
#include "Particle/ParticleManager.h"
#include "Spread/SpreadManager.h"
#include "Terrain/Terrain.h"
#include "Player.h"
using namespace glm;

static constexpr int INIT_TIME = 60 * 3;
static constexpr int RISE_TIME = 10;
static constexpr int END_TIME = INIT_TIME + RISE_TIME;
static constexpr float DIG_HEIGHT = 8.0f;
static constexpr float EXTEND_HEIGHT = 1.0f;

EntityDependendies RisePillar::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void RisePillar::Init(Entity::InitArgs args) {
    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_Overlap, true); 
    SetFlag(EF_Homeable, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color_terrainclipped");
    materials_[0].transparencyType = TRANSPARENCY_UNORDERED;
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 0.75f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.15f;

    overlapbox_.top = 1.5f;
    overlapbox_.bottom = -0.75f;
    overlapbox_.radius = 1.5f;

    ParticleManager& particleManager = args.particleManager;
    burrowEm_ = particleManager.RequestEmitter();
    burrowEm_->properties_.material.shader = resourceManager.GetShader("vs_particle", "fs_particle");
    burrowEm_->properties_.material.transparencyType = TRANSPARENCY_ADDITIVE;
    burrowEm_->properties_.material.castShadows = false;
    burrowEm_->properties_.material.triangleType = TriangleType::TWO_SIDED;
    burrowEm_->properties_.spawnInterval = 0.005f;
    burrowEm_->properties_.spawnCount = 10;
    burrowEm_->properties_.spawnRadius = 0.25f;
    burrowEm_->properties_.lifetime = 0.25f;
    burrowEm_->properties_.minScale = 0.5f;
    burrowEm_->properties_.maxScale = 1.0f;
    burrowEm_->properties_.endScale = 1.5f;
    burrowEm_->properties_.minVelocity = vec4(-30.0f, 0.5f, -30.0f, 0.0f);
    burrowEm_->properties_.maxVelocity = vec4(30.0f, 10.5f, 30.0f, 0.0f);
    burrowEm_->properties_.startColor = vec4(1.0f, 1.0f, 1.0f, 0.75f);
    burrowEm_->properties_.endColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);

    shootEm_ = particleManager.RequestEmitter();
    shootEm_->properties_.emissionType = EmitterProperties::ET_PlanarDisc;
    shootEm_->properties_.terrainCollision = true;
    shootEm_->properties_.material.shader = resourceManager.GetShader("vs_particle", "fs_particle");
    shootEm_->properties_.material.transparencyType = TRANSPARENCY_ADDITIVE;
    shootEm_->properties_.material.castShadows = false;
    shootEm_->properties_.material.triangleType = TriangleType::TWO_SIDED;
    shootEm_->properties_.spawnCount = 40;
    shootEm_->properties_.spawnRadius = 0.25f;
    shootEm_->properties_.lifetime = 0.25f;
    shootEm_->properties_.minScale = 1.0f;
    shootEm_->properties_.maxScale = 1.5f;
    shootEm_->properties_.endScale = 4.0f;
    shootEm_->properties_.minVelocity = vec4(80.0f, -10.0f, 0.0f, 0.0f);
    shootEm_->properties_.maxVelocity = vec4(100.0f, -10.0f, 0.0f, 0.0f);
    shootEm_->properties_.damping = vec4(4.0f, 0.0f, 4.0f, 0.0f);
    shootEm_->properties_.startColor = vec4(1.0f, 1.0f, 1.0f, 0.75f);
    shootEm_->properties_.endColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);

    burstEm_ = particleManager.RequestEmitter();
    burstEm_->properties_.emissionType = EmitterProperties::ET_PlanarDisc;
    burstEm_->properties_.material.shader = resourceManager.GetShader("vs_particle_stretch", "fs_particle");
    burstEm_->properties_.material.transparencyType = TRANSPARENCY_ADDITIVE;
    burstEm_->properties_.material.castShadows = false;
    burstEm_->properties_.material.triangleType = TriangleType::TWO_SIDED;
    burstEm_->properties_.spawnCount = 40;
    burstEm_->properties_.spawnRadius = 0.25f;
    burstEm_->properties_.lifetime = 0.15f;
    burstEm_->properties_.minScale = 0.5f;
    burstEm_->properties_.maxScale = 1.0f;
    burstEm_->properties_.endScale = 0.0f;
    burstEm_->properties_.minVelocity = vec4(80.0f, 120.0f, 0.0f, 0.0f);
    burstEm_->properties_.maxVelocity = vec4(110.0f, 140.0f, 0.0f, 0.0f);
    burstEm_->properties_.acceleration = vec4(0.0f, -400.0f, 0.0f, 0.0f);
    burstEm_->properties_.damping = vec4(4.0f, 0.0f, 4.0f, 0.0f);
    burstEm_->properties_.startColor = vec4(1.0f, 1.0f, 1.0f, 0.75f);
    burstEm_->properties_.endColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);

    timer_ = 0;
    initialY_ = transform_.position.y;

}

void RisePillar::Update() {
    if (timer_ <= END_TIME)
        timer_++;

    if (timer_ < INIT_TIME)
        transform_.position.y = std::lerp(initialY_, initialY_ + DIG_HEIGHT, timer_ / (float)INIT_TIME);

    if (timer_ >= INIT_TIME && timer_ <= END_TIME) { 
        float t = EaseQuad((timer_ - INIT_TIME) / (float)RISE_TIME);
        transform_.position.y = std::lerp(initialY_ + DIG_HEIGHT, initialY_ + EXTEND_HEIGHT * transform_.scale.y, t);
    }

    if (timer_ == END_TIME) {
        float spreadRadius = pushbox_.radius * max(transform_.scale.x, transform_.scale.z) * 5.0f;
        spreadManager_->AddSpread(transform_.position, spreadRadius, 0.65f, SpreadManager::AD_Feather, SpreadType_Weed);
    }
}

void RisePillar::RenderUpdate() {
    shootEm_->active_ = false;
    burrowEm_->active_ = false;
    burstEm_ ->active_ = false;

    if (timer_ >= INIT_TIME && timer_ <= END_TIME) {
        float t = 1.0f - EaseCubic((timer_ - INIT_TIME) / (float)RISE_TIME);
        renderTransform_.position.x += pow(sin(GlobalTime::GetTime() * 40.0f + 8.0f), 2.0f) * 0.5f * t;
        renderTransform_.position.y += pow(sin(GlobalTime::GetTime() * 40.0f + 16.0f), 2.0f) * 0.5f * t;
        renderTransform_.position.z += pow(sin(GlobalTime::GetTime() * 40.0f + 32.0f), 2.0f) * 0.5f * t;
    }
    else if (timer_ < INIT_TIME) {
        renderTransform_.position.x += pow(sin(GlobalTime::GetTime() * 40.0f + 8.0f), 2.0f) * 0.5f;
        renderTransform_.position.y += pow(sin(GlobalTime::GetTime() * 40.0f + 16.0f), 2.0f) * 0.5f;
        renderTransform_.position.z += pow(sin(GlobalTime::GetTime() * 40.0f + 32.0f), 2.0f) * 0.5f;
        burrowEm_->active_ = true;
    }

    if (timer_ >= INIT_TIME && timer_ <= INIT_TIME + 3) {
        shootEm_->active_ = true;
        burstEm_->active_ = true;
    }

    burrowEm_->transform_ = renderTransform_;
    burrowEm_->transform_.position.y = terrain_->GetHeight(burrowEm_->transform_.position, TA_Low);
    shootEm_->transform_ = renderTransform_;
    shootEm_->transform_.position.y = terrain_->GetHeight(shootEm_->transform_.position, TA_Low);
    burstEm_->transform_ = renderTransform_;
    burstEm_->transform_.position.y = terrain_->GetHeight(burstEm_->transform_.position, TA_Low) - 5.0f;
}

void RisePillar::OnDestroy() {
    burrowEm_->release_ = true;
    shootEm_->release_ = true;
    burstEm_->release_ = true;
}

void RisePillar::OnHitlagEnd() {
    destroy_ = true;
}

void RisePillar::OnOverlap(Entity* overlappedEntity) {
    Player* player = nullptr;
    if (overlappedEntity->typeId_ == Player::TYPEID)
        player = (Player*)overlappedEntity;

    bool homing = false;
    if (player != nullptr && player->homingTarget_ == nullptr)
        homing = true;

    if (overlappedEntity->velocity_.y > 0.0f && !homing)
        return;

    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = 70.0f;

    if (player != nullptr)
        player->EndHoming();
}

vec3 RisePillar::GetTargetPoint() {
    return transform_.position + vec3(0.0f, transform_.scale.y, 0.0f);
}
