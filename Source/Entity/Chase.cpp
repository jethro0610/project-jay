#include "Chase.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Entity/Player.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
#include "Seed/SeedManager.h"
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
            {"p_seeds", &numSeeds_},
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
    numSeeds_ = 300;

    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_Trackable, true);
    SetFlag(EF_RecievePush, true);
}

void Chase::Start() {
    player_ = (Player*)entities_->FindEntityByType(Player::TYPEID);
    spawnPoint_ = transform_.position;
}

void Chase::Update() {
    if (hopping_ | returning_)
        velocity_.y -= 4.0f;
    else
        velocity_.y -= 2.0f;

    vec2 terrainDist = terrain_->GetRawDistance(transform_.position);
    bool shouldReturn = false;
    if (terrainDist.x > 0.0f)
        shouldReturn = true;
    if (transform_.position.y - terrainDist.y > 0.25f)
        shouldReturn = false;
    if (!onGround_ && velocity_.y > 0.0f)
        shouldReturn = false;

    if (shouldReturn) {
        SetFlag(EF_RecieveHits, false);
        vec3 directionToSpawn = spawnPoint_ - transform_.position;
        directionToSpawn.y = 0.0f;
        directionToSpawn = normalize(directionToSpawn);
        skipGroundCheck_ = true;
        velocity_ = directionToSpawn * 250.0f;
        velocity_.y = 225.0f;
        returning_ = true;
        hopping_ = false;
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
            hopping_ = true;
            skipGroundCheck_ = true;
            return;
        }
    }

    if (returning_) {
        returning_ = false;
        SetFlag(EF_RecieveHits, true);
    }
    hopping_ = false;
    traceDistance_ = 20.0f;

    float speedDecay = 1.0f - friction_;
    float acceleration = (speed_ / speedDecay) - speed_;

    float distToPlayer2 = distance2(player_->transform_.position, transform_.position);
    float distToSpawn2 = distance2(spawnPoint_, transform_.position);
    if (distToPlayer2 > 190.0f * 190.0f) {
        if (walkToCenterTimer_ > 0)
            walkToCenterTimer_--;
    }
    else {
        walkToCenterTimer_ = RandomIntRange(60 * 5, 60 * 12);
    }

    if (distToPlayer2 < 150.0f * 150.0f) {
        vec3 directionToPlayer = player_->transform_.position - transform_.position;
        directionToPlayer.y = 0.0f;
        directionToPlayer = normalize(directionToPlayer);
        velocity_ += -directionToPlayer * acceleration;
    }
    else if (distToSpawn2 > 20.0f * 20.0f && !stun_ && walkToCenterTimer_ <= 0) {
        vec3 directionToSpawn = spawnPoint_ - transform_.position;
        directionToSpawn.y = 0.0f;
        directionToSpawn = normalize(directionToSpawn);
        velocity_ += directionToSpawn * acceleration;
    }

    vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
    planarVelocity *= speedDecay;
    velocity_.x = planarVelocity.x;
    velocity_.z = planarVelocity.z;
}

void Chase::OnHurt(HurtArgs args) {
    stunEndTimer_ = 20;
    seedManager_->CreateMultipleSeed(transform_.position, numSeeds_, 25.0f, args.attacker);
}
