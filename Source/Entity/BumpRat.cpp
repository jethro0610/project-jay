#include "BumpRat.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
#include "Rendering/Material.h"
#include "Spread/SpreadManager.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

EntityDependendies BumpRat::GetDeps() {
    return {
        "sk_spinrat",
        {
            "t_spinrat_back_m",
            "t_spinrat_swirl_m",
            "t_spinrat_nose_m",
            "t_spinrat_eyes_m",
            "t_spinrat_ears_f_m",
            "t_spinrat_ears_b_m",
        },
    };
}

void BumpRat::Init(Entity::InitArgs args) {
    Entity::Init(args);
    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecievePush, true);
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
        materials_[i].selectedShader = resourceManager.GetShader("vs_skeletal", "fs_selected");
        materials_[i].properties = MaterialProperties::Default();
    }

    materials_[BACK].shader = resourceManager.GetShader("vs_skeletal", "fs_color_masked");
    materials_[BACK].castShadows = false;
    materials_[BACK].properties.color = LINECOLOR;
    materials_[BACK].numTextures = 1;
    materials_[BACK].textures[0] = resourceManager.GetTexture("t_spinrat_back_m");

    materials_[BODY].shader = resourceManager.GetShader("vs_skeletal", "fs_aura");
    materials_[BODY].shadowShader = resourceManager.GetShader("vs_skeletal_s", "fs_depth_s");
    materials_[BODY].castShadows = true;
    materials_[BODY].properties.color = BODYCOLOR;
    materials_[BODY].properties.fresnelPower = 0.15f;

    materials_[FRONTEARS].shader = resourceManager.GetShader("vs_skeletal", "fs_color_masked");
    materials_[FRONTEARS].castShadows = false;
    materials_[FRONTEARS].properties.color = LINECOLOR;
    materials_[FRONTEARS].numTextures = 1;
    materials_[FRONTEARS].textures[0] = resourceManager.GetTexture("t_spinrat_ears_f_m");

    materials_[BACKEARS].shader = resourceManager.GetShader("vs_skeletal", "fs_color_masked");
    materials_[BACKEARS].castShadows = false;
    materials_[BACKEARS].properties.color = LINECOLOR;
    materials_[BACKEARS].numTextures = 1;
    materials_[BACKEARS].textures[0] = resourceManager.GetTexture("t_spinrat_ears_b_m");

    materials_[EYES].shader = resourceManager.GetShader("vs_skeletal", "fs_color_masked");
    materials_[BACK].castShadows = false;
    materials_[EYES].properties.color = LINECOLOR;
    materials_[EYES].numTextures = 1;
    materials_[EYES].textures[0] = resourceManager.GetTexture("t_spinrat_eyes_m");

    materials_[NOSE].shader = resourceManager.GetShader("vs_skeletal", "fs_color_masked");
    materials_[NOSE].castShadows = false;
    materials_[NOSE].properties.color = LINECOLOR;
    materials_[NOSE].numTextures = 1;
    materials_[NOSE].textures[0] = resourceManager.GetTexture("t_spinrat_nose_m");

    materials_[BELLY].shader = resourceManager.GetShader("vs_skeletal", "fs_color_masked");
    materials_[BELLY].castShadows = false;
    materials_[BELLY].properties.color = LINECOLOR;
    materials_[BELLY].numTextures = 1;
    materials_[BELLY].textures[0] = resourceManager.GetTexture("t_spinrat_swirl_m");

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

    pushbox_.radius = 3.0f;
    pushbox_.top = 12.0f;
    pushbox_.bottom = 0.0f;
    #ifdef _DEBUG
    DBG_collider_ = pushbox_;
    #endif

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
        spreadManager_->RemoveSpread(transform_.position, 6);
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
    seedManager_->CreateMultipleSeed(transform_.position, 300, 20.0f);
}
