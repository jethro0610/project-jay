#include "BumpRat.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
#include "Resource/ResourceManager.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

void BumpRat::Init(EntityS::InitArgs args) {
    EntityS::Init(args);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_UseSkeleton, true);
    SetFlag(EF_Interpolate, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("sk_spinrat");
    skeleton_ = resourceManager.GetSkeleton("sk_spinrat");
    materials_[0] = resourceManager.GetMaterial("m_spinrat_back");
    materials_[1] = resourceManager.GetMaterial("m_spinrat_body");
    materials_[2] = resourceManager.GetMaterial("m_spinrat_ears_b");
    materials_[3] = resourceManager.GetMaterial("m_spinrat_ears_f");
    materials_[4] = resourceManager.GetMaterial("m_spinrat_eyes");
    materials_[5] = resourceManager.GetMaterial("m_spinrat_nose");
    materials_[6] = resourceManager.GetMaterial("m_spinrat_swirl");

    pose_.resize(skeleton_->bones_.size());
    renderPose_.resize(skeleton_->bones_.size());

    vec3 centerOffset = RandomVectorPlanar(100.0f);
    desiredMovement_ = normalize(centerOffset - vec3(transform_.position.x, 0.0f, transform_.position.z));

    ChangeAnimation(2, 0.0f);
}

void BumpRat::Update() {

    // entity.emitters_[0]->active_ = false;
    // if (groundTraceComponent.onGround[entityId])
    //     entity.emitters_[0]->active_ = true;

    // hitboxComponent.hitboxes[entityId][0].active = true;

    // if (hurtboxComponent.hurt[entityId]) {
    //     desiredMovement = -normalize(vec3(velocity.x, 0.0f, velocity.z));
    //     skeletonComponent.nextAnimationIndex[entityId] = 5;
    //     hitboxComponent.hitboxes[entityId][0].active = false;
    // }
    // else if (hurtboxComponent.stun[entityId]) {
    //     skeletonComponent.nextAnimationIndex[entityId] = 4;
    //     hitboxComponent.hitboxes[entityId][0].active = false;
    //     return;
    // }
    // else if (hitboxComponent.hit[entityId]){
    //     skeletonComponent.nextAnimationIndex[entityId] = 6;
    // }
    // else
    //     skeletonComponent.nextAnimationIndex[entityId] = 2;


    float curDistance = terrain_->GetDistance(transform_.position).x;
    float forwardDistance = terrain_->GetDistance(transform_.position + desiredMovement_ * 10.0f).x;

    desiredMovement_ = rotate(desiredMovement_, RandomFloatRange(-0.1f, 0.1f), Transform::worldUp);

    if (curDistance > -50.0f && forwardDistance - curDistance > 0.0f) {
        vec3 centerOffset = RandomVectorPlanar(100.0f);
        desiredMovement_ = normalize(centerOffset - vec3(transform_.position.x, 0.0f, transform_.position.z));
    }

    velocity_.x += desiredMovement_.x * ACCELERATION;
    velocity_.z += desiredMovement_.z * ACCELERATION;
    velocity_.x *= SPEED_DECAY;
    velocity_.z *= SPEED_DECAY;
    if (length(desiredMovement_) > 0.001f) {
        quat rotation = quatLookAtRH(normalize(desiredMovement_), Transform::worldUp);
        transform_.rotation = slerp(transform_.rotation, rotation, ROTATION_SPEED);
    }
}
