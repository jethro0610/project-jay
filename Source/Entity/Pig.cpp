#include "Pig.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
#include "Apple.h"
#include "Terrain/Terrain.h"
#include "Seed/SeedManager.h"
#include "Helpers/Random.h"
using namespace glm;

EntityDependendies Pig::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

static constexpr int MIN_WANDER_WAIT = 30;
static constexpr int MAX_WANDER_WAIT = 90;
void Pig::Init(Entity::InitArgs args) {
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_HurtFaceForward, true);
    SetFlag(EF_SendHits, true);
    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecievePush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.75f, 0.75f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hitbox_.top = 1.0f;
    hitbox_.bottom = 1.0f;
    hitbox_.radius = 1.25f;

    hurtbox_.top = 1.5f;
    hurtbox_.bottom = 1.5f;
    hurtbox_.radius = 1.75f;

    target_ = nullptr;
    seeds_ = 0;
    origin_ = transform_.position;
    wanderTarget_ = vec3(0.0f);
    wandering_ = false;
    wanderTimer_ = RandomIntRange(MIN_WANDER_WAIT, MAX_WANDER_WAIT);
}

static constexpr float SPEED = 100.0f;
static constexpr float WANDER_SPEED = 20.0f;
static constexpr float FRICTION = 0.15f;
static constexpr float SPEED_DECAY = 1.0f - FRICTION;
static constexpr float ACCELERATION = ((SPEED / SPEED_DECAY) - SPEED);
static constexpr float WANDER_ACCELERATION = ((WANDER_SPEED / SPEED_DECAY) - WANDER_SPEED);

void Pig::Update() {
    vec3 desiredMovement = vec3(0.0f);
    if (target_ != nullptr && !target_->alive_)
        target_ = nullptr;

    velocity_.y -= 1.0f;
    if (onGround_)
        stun_ = false;

    // Start wandering once done waiting for wander
    if (wanderTimer_ <= 0 && !wandering_) {
        wandering_ = true;
        wanderTarget_ = terrain_->GetRandomPointInSameIsland(origin_, 20.0f, 160.0f);
    }

    // Move towards target if wandering, or tick the timer down
    if (wandering_ == true)
        desiredMovement = normalize(wanderTarget_ - transform_.position);
    else
        wanderTimer_--;

    // Start wander waiting once we get to the target
    if (distance(transform_.position, wanderTarget_) < 5.0f && wandering_) {
        wandering_ = false;
        wanderTimer_ = RandomIntRange(MIN_WANDER_WAIT, MAX_WANDER_WAIT);
    }

    if (target_ == nullptr) {
        FindTargetApple();
    }
    else {
        wanderTimer_ = 0;
        wandering_ = false;
        desiredMovement = normalize(target_->transform_.position - transform_.position);
    }


    float acceleration = target_ == nullptr ? WANDER_ACCELERATION : ACCELERATION;
    if (!stun_) {
        velocity_.x += desiredMovement.x * acceleration;
        velocity_.z += desiredMovement.z * acceleration;
        velocity_.x *= SPEED_DECAY;
        velocity_.z *= SPEED_DECAY;
    }

    if (stun_)
        SetFlag(EF_SendPush, false);
    else
        SetFlag(EF_SendPush, true);
}

void Pig::FindTargetApple() {
    float closestTargetDist = INFINITY;
    Apple* target = nullptr;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity* entity = &(*entities_)[i];
        if (!entity->alive_)
            continue;

        if (entity->typeId_ != Apple::TYPEID)
            continue;

        Apple* apple = (Apple*)entity;
        if (!apple->active_)
            continue;
        if (!apple->onGround_)
            continue;

        bool appleAlreadyTargeted = false;
        for (int j = 0; j < EntityList::MAX; j++) {
            if (j == i)
                continue;
            Entity* possiblePig = &(*entities_)[j];
            if (!possiblePig->alive_)
                continue;
            if (possiblePig->typeId_ != Pig::TYPEID)
                continue;

            Pig* pig = (Pig*)possiblePig;
            if (pig->target_ == apple)
                appleAlreadyTargeted = true;
        }
        if (appleAlreadyTargeted)
            continue;

        if (!terrain_->PointIsInSameIsland(transform_.position, apple->transform_.position))
            continue;

        float dist = distance(transform_.position, entity->transform_.position);
        if (dist < closestTargetDist) {
            target = apple;
            closestTargetDist = dist;
        }
    }
    target_ = target;
}

void Pig::OnCaptureSeed() {
    seeds_++;
}

static constexpr int NUM_SEEDS_ON_HIT = 150;
void Pig::OnHurt(HurtArgs args) {
    int numSeeds = min(NUM_SEEDS_ON_HIT, seeds_);
    seedManager_->CreateMultipleSeed(transform_.position, numSeeds, 12.0f, args.attacker);
    seeds_ -= numSeeds;
}
