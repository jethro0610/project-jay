#include "BumpRat.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
#include "Resource/ResourceManager.h"
#include "Rendering/Material.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

void BumpRat::Init(Entity::InitArgs args) {
    Entity::Init(args);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_UseSkeleton, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_SendHits, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_HurtFaceBack, true);
    
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("sk_spinrat");
    skeleton_ = resourceManager.GetSkeleton("sk_spinrat");
    for (int i = 0; i < 7; i++) {
        materials_[i].shader = resourceManager.GetShader("vs_skeletal", "fs_dfsa_color");
        materials_[i].shadowShader = resourceManager.GetShader("vs_skeletal_s", "fs_depth_s");
        materials_[i].castShadows = true;

        materials_[i].properties[MPROP_SPEC_POWER] = 32.0f;
        materials_[i].properties[MPROP_SPEC_THRESH] = 0.3f;
        materials_[i].properties[MPROP_SPEC_BRIGHTNESS] = 1.5f;

        materials_[i].properties[MPROP_FRES_POWER] = 4.0f;
        materials_[i].properties[MPROP_FRES_SCALE] = 1.0f;
        materials_[i].properties[MPROP_FRES_BRIGHTNESS] = 1.0f;

        materials_[i].properties[MPROP_TEXSCALE_X] = 1.0f;
        materials_[i].properties[MPROP_TEXSCALE_Y] = 1.0f;

        materials_[i].properties[MPROP_R] = 0.75f;
        materials_[i].properties[MPROP_G] = 0.75f;
        materials_[i].properties[MPROP_B] = 0.75f;
    }
    // materials_[0] = resourceManager.GetMaterial("m_spinrat_back");
    // materials_[1] = resourceManager.GetMaterial("m_spinrat_body");
    // materials_[2] = resourceManager.GetMaterial("m_spinrat_ears_b");
    // materials_[3] = resourceManager.GetMaterial("m_spinrat_ears_f");
    // materials_[4] = resourceManager.GetMaterial("m_spinrat_eyes");
    // materials_[5] = resourceManager.GetMaterial("m_spinrat_nose");
    // materials_[6] = resourceManager.GetMaterial("m_spinrat_swirl");

    pose_.resize(skeleton_->bones_.size());
    renderPose_.resize(skeleton_->bones_.size());

    hitbox_.radius = 4.0f;
    hitbox_.top = 2.0f;
    hitbox_.bottom = 0.0f;
    hitbox_.active = true;
    hitbox_.hitlag = 6;
    hitbox_.forwardRange = 0.25f;

    hurtbox_.radius = 6.0f;
    hurtbox_.top = 16.0f;
    hurtbox_.bottom = 0.0f;

    vec3 centerOffset = RandomVectorPlanar(100.0f);
    desiredMovement_ = normalize(centerOffset - vec3(transform_.position.x, 0.0f, transform_.position.z));
    attackStartup_ = STARTUP_TIME;

    ChangeAnimation(2, 0.35f);
}

void BumpRat::Update() {
    float curDistance = terrain_->GetDistance(transform_.position).x;
    float forwardDistance = terrain_->GetDistance(transform_.position + desiredMovement_ * 10.0f).x;

    desiredMovement_ = rotate(desiredMovement_, RandomFloatRange(-0.1f, 0.1f), Transform::worldUp);

    if (curDistance > -50.0f && forwardDistance - curDistance > 0.0f) {
        vec3 centerOffset = RandomVectorPlanar(100.0f);
        desiredMovement_ = normalize(centerOffset - vec3(transform_.position.x, 0.0f, transform_.position.z));
    }

    if (onGround_) {
        if (stun_) {
            stun_ = false;
            desiredMovement_ = transform_.GetForwardVector();
        }
        if (attackStartup_ < STARTUP_TIME)
            attackStartup_++;
    }
    else {
        hitbox_.active = false;
        attackStartup_ = 0;
    }

    if (attackStartup_ == STARTUP_TIME)
        hitbox_.active = true;

    velocity_.y -= 1.0f;
    if (!stun_) {
        velocity_.x += desiredMovement_.x * ACCELERATION;
        velocity_.z += desiredMovement_.z * ACCELERATION;
        velocity_.x *= SPEED_DECAY;
        velocity_.z *= SPEED_DECAY;
        if (length(desiredMovement_) > 0.001f) {
            quat rotation = quatLookAtRH(normalize(desiredMovement_), Transform::worldUp);
            transform_.rotation = slerp(transform_.rotation, rotation, ROTATION_SPEED);
        }
    }

    hitbox_.knocback = transform_.GetForwardVector() * 65.0f;
    hitbox_.knocback.y = 35.0f;

    int animation = 2;
    float transitionLength = 0.35f;
    if (stun_) {
        animation = 4;
        transitionLength = 0.15f;
    }
    if (animation != animIndex_)
        ChangeAnimation(animation, transitionLength);
}

void BumpRat::OnHit() {
    ChangeAnimation(6, 0.0f);
}

void BumpRat::OnHurt() {
    ChangeAnimation(5, 0.0f);
}
