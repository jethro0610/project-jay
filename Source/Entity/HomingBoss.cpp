#include "HomingBoss.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "EntityList.h"
#include "HomingBossPoint.h"
#include "HomingBossAttack.h"
#include "Helpers/PhysicsHelpers.h"
#include "Helpers/Random.h"
#include "Camera/Camera.h"
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

EntityDependendies HomingBoss::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HomingBoss::GetStaticProperties() {
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

void HomingBoss::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(1.0f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 2.0f;
    overlapbox_.bottom = 2.0f;
    overlapbox_.radius = 2.0f;

    phase_ = 0;
    damage_ = 0;
    overlapCooldown_ = 0;
    player_ = nullptr;
    centerPointRot_ = 0;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_Homeable, true);
    SetFlag(EF_RecievePush, true);
    SetFlag(EF_Interpolate, true);
}

void HomingBoss::Start() {
    centerPoint_ = transform_.position;
    int numPointsFound = entities_->FindEntitiesByType(HomingBossPoint::TYPEID, (Entity**)points_, NUM_POINTS);
    assert(numPointsFound == NUM_POINTS);

    int numAttacksFound = entities_->FindEntitiesByType(HomingBossAttack::TYPEID, (Entity**)attacks_, NUM_ATTACKS);
    assert(numAttacksFound == NUM_ATTACKS);

    player_ = (Player*)entities_->FindEntityByType(Player::TYPEID);
    assert(player_ != nullptr);

    for (int i = 0; i < NUM_ATTACKS; i++) {
        attacks_[i]->homingBoss_ = this; 
        attacks_[i]->player_ = player_;
        attacks_[i]->Deactivate();
    }

    for (int i = 0; i < 2; i++)
        ActivateRandomAttack();
}

void HomingBoss::Update() {
    if (overlapCooldown_ > 0)
        overlapCooldown_--;

    centerPointRot_ += 0.01f;
    vec3 rotVec = vec3(sin(centerPointRot_), 0.0f, cos(centerPointRot_));
    float y = sin(centerPointRot_ * 2.25f) * 20.0f;

    float distT = (sin(centerPointRot_ * 1.25f) + 1.0f) * 0.5f;
    float dist = mix(150.0f, 225.0f, distT);

    transform_.position = centerPoint_ + rotVec * dist;
    transform_.position.y += y;
}

void HomingBoss::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ == Player::TYPEID)
        ((Player*)overlappedEntity)->EndHoming();
    else
        return;

    if (overlapCooldown_ > 0)
        return;

    overlapCooldown_ = 20;
    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = phase_ > 0 ? 200.0f : 125.0f;

    // Beyond phase 1, we need to hit all 
    // points before considering this a hit
    if (phase_ > 0 && !AllPointsHit())
        return;

    hitlag_ = 8;
    stun_ = true;
    overlappedEntity->hitlag_ = 8;

    damage_++;
    if (phase_ == 0) {
        if (damage_ >= 6) {
            phase_++;
            ActivatePoints();
        }
    }
    else if (phase_ == 1) {
        ActivatePoints();
    }
}

void HomingBoss::ActivatePoints() {
    int startIndex = -1;
    float maxDot = -INFINITY;
    for (int i = 0; i < NUM_POINTS; i++) {
        vec3 dirToPoint = normalize(points_[i]->transform_.position - camera_->transform_.position);
        float d = dot(camera_->transform_.GetForwardVector(), dirToPoint);
        if (d > maxDot) {
            startIndex = i;
            maxDot = d;
        }
    }

    for (int i = 0; i < NUM_POINTS; i++) {
        int index = (i + startIndex) % NUM_POINTS;
        points_[i]->ActivateInTicks(30 + i * 40);
    }
}

bool HomingBoss::AllPointsHit() {
    for (int i = 0; i < NUM_POINTS; i++) {
        if (!points_[i]->hit_) 
            return false;
    }
    return true;
}

void HomingBoss::OnAttackHit() {
    ActivateRandomAttack();
}

void HomingBoss::ActivateRandomAttack() {
    int activateIndex = -1;
    do {
        activateIndex = rand() % NUM_ATTACKS;
    }
    while (!IsValidAttack(attacks_[activateIndex]));
    attacks_[activateIndex]->Activate(); 
}

bool HomingBoss::IsValidAttack(HomingBossAttack* attack) {
    if (attack->active_)
        return false;

    // if (distance2(attack->transform_.position, player_->transform_.position) < 300.0f * 300.0f)
    //     return false;

    for (int i = 0; i < NUM_ATTACKS; i++) {
        if (attacks_[i] == attack) continue;
        if (!attacks_[i]->active_) continue;
        if (distance2(attack->transform_.position, attacks_[i]->transform_.position) < 300.0f * 300.0f)
            return false;
    }
    return true;
}
