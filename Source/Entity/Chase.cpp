#include "Chase.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Entity/Player.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
using namespace glm;

EntityDependendies Chase::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Chase::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_speed", &speed_},
            {"p_friction", &friction_},
        },
        {
            // Ints
        },
        {
            // Bools
        }
    };
}

void Chase::Init(Entity::InitArgs args) {
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
    hurtbox_.radius = 1.25f;
    
    friction_ = 0.05f;
    speed_ = 50.0f;
    player_ = nullptr;
    returning_ = false;
    spawnPoint_ = transform_.position;

    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_Trackable, true);
}

void Chase::Start() {
    player_ = (Player*)entities_->FindEntityByType(Player::TYPEID);
    spawnPoint_ = transform_.position;
}

void Chase::Update() {
    velocity_.y -= returning_ ? 3.0f : 2.0f;
    vec2 terrainDist = terrain_->GetRawDistance(transform_.position);
    if (terrainDist.x > -30.0f && (transform_.position.y - terrainDist.y) < 0.25f && !returning_) {
        SetFlag(EF_RecieveHits, false);
        vec3 directionToSpawn = spawnPoint_ - transform_.position;
        directionToSpawn.y = 0.0f;
        directionToSpawn = normalize(directionToSpawn);
        skipGroundCheck_ = true;
        velocity_ = directionToSpawn * 200.0f;
        velocity_.y = 200.0f;
        returning_ = true;
        stun_ = false;
        return;
    }

    if (!onGround_ || skipGroundCheck_) {
        traceDistance_ = 1.0f;
        return;
    }

    if (stun_) {
        stunEndTimer_--;
        if (stunEndTimer_ <= 0) {
            stun_ = false;
            vec3 hopVel = RandomVectorPlanar(100.0f, 150.0f);
            velocity_ = hopVel;
            velocity_.y = 150.0f;
            skipGroundCheck_ = true;
            return;
        }
    }

    if (returning_) {
        returning_ = false;
        SetFlag(EF_RecieveHits, true);
    }
    traceDistance_ = 20.0f;

    float speedDecay = 1.0f - friction_;
    float acceleration = (speed_ / speedDecay) - speed_;

    float distToPlayer2 = distance2(player_->transform_.position, transform_.position);
    if (distToPlayer2 < 300.0f * 300.0f) {
        vec3 directionToPlayer = player_->transform_.position - transform_.position;
        directionToPlayer.y = 0.0f;
        directionToPlayer = normalize(directionToPlayer);
        velocity_ += -directionToPlayer * acceleration;
    }

    velocity_ *= speedDecay;
}

void Chase::OnHurt(HurtArgs args) {
    stunEndTimer_ = 20;
}
