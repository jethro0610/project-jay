#include "HomingBoss.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "EntityList.h"
#include "HomingBossPoint.h"
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

    SetFlag(EF_Overlap, true);
    SetFlag(EF_Homeable, true);
}

void HomingBoss::Start() {
    int numFound = entities_->FindEntitiesByType(HomingBossPoint::TYPEID, (Entity**)points_, NUM_POINTS);
    assert(numFound == NUM_POINTS);
}

void HomingBoss::Update() {
    if (overlapCooldown_ > 0)
        overlapCooldown_--;
}

void HomingBoss::OnOverlap(Entity* overlappedEntity) {
    if (overlapCooldown_ > 0)
        return;

    overlapCooldown_ = 20;
    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = 225.0f;

    if (overlappedEntity->typeId_ == Player::TYPEID)
        ((Player*)overlappedEntity)->EndHoming();

    // Beyond phase 1, we need to hit
    // all points before considering this
    // a hit
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
