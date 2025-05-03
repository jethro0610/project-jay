#include "HomingBoss.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "EntityList.h"
#include "HomingBossPoint.h"
#include "Helpers/PhysicsHelpers.h"
#include "HomingBossProjectile.h"
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
    projectileTimer_ = 0;
    projectile_ = nullptr;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_Homeable, true);
    SetFlag(EF_RecievePush, true);
    SetFlag(EF_SendPush, true);
}

void HomingBoss::Start() {
    int numFound = entities_->FindEntitiesByType(HomingBossPoint::TYPEID, (Entity**)points_, NUM_POINTS);
    assert(numFound == NUM_POINTS);

    player_ = (Player*)entities_->FindEntityByType(Player::TYPEID);
    assert(player_ != nullptr);
}

void HomingBoss::Update() {
    if (projectile_ != nullptr && !projectile_->alive_)
        projectile_ = nullptr;

    if (overlapCooldown_ > 0)
        overlapCooldown_--;

    if (projectile_ != nullptr)
        projectileTimer_ = 0;

    projectileTimer_++; 

    if (
        projectile_ == nullptr && 
        projectileTimer_ >= 60 * 3 && 
        distance(player_->transform_.position, transform_.position) < 1200.0f &&
        player_->onGround_
    ) {
        LaunchProjectile();
        projectileTimer_ = 0;
    }
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
    overlappedEntity->velocity_.y = 225.0f;

    // Beyond phase 1, we need to hit all 
    // points before considering this a hit
    if (phase_ > 0 && !AllPointsHit())
        return;

    hitlag_ = 8;
    stun_ = true;
    overlappedEntity->hitlag_ = 8;

    damage_++;
    if (phase_ == 0) {
        if (damage_ >= 3) {
            phase_++;
            ActivatePoints();
        }
    }
    else if (phase_ == 1) {
        ActivatePoints();
    }
}

void HomingBoss::ActivatePoints() {
    for (int i = 0; i < NUM_POINTS; i++)
        points_[i]->Activate();
}

bool HomingBoss::AllPointsHit() {
    for (int i = 0; i < NUM_POINTS; i++) {
        if (!points_[i]->hit_) 
            return false;
    }
    return true;
}

void HomingBoss::LaunchProjectile() {
    vec3 velocity = GetProjectileVelocityToPoint(
        transform_.position, 
        player_->transform_.position, 
        HomingBossProjectile::GRAVITY, 
        HomingBossProjectile::LAUNCH_STRENGTH
    );
    if (velocity != vec3(0.0f)) {
        Transform projTransform;
        projTransform.scale = vec3(8.0f);
        projTransform.position = transform_.position;
        projectile_ = (HomingBossProjectile*)(&entities_->CreateEntity(HomingBossProjectile::TYPEID, projTransform));
        projectile_->velocity_ = velocity;
    }
}
