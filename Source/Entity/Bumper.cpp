#include "Bumper.h"
#include "Resource/ResourceManager.h"
using namespace glm;

EntityDependendies Bumper::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Bumper::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_friction", &friction_}
        },
        {
            // Ints
        },
        {
            // Bools
        }
    };
}

void Bumper::Init(Entity::InitArgs args) {
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
    hurtbox_.radius = 1.5f;
    
    friction_ = 0.25f;
    speed_ = 5.0f;
    traceDistance_ = 20.0f;

    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_RecieveKnockback, true);
    SetFlag(EF_CustomKnockback, true);
    SetFlag(EF_Trackable, true);
    SetFlag(EF_RecievePush, true);
}

void Bumper::Update() {
    float speedDecay = 1.0f - friction_;
    float acceleration = (speed_ / speedDecay) - speed_;
    velocity_.y -= 4.0f;

    vec3 planarVelocity = vec3(velocity_.x, 0.0f, velocity_.z);
    planarVelocity *= speedDecay;
    velocity_.x = planarVelocity.x;
    velocity_.z = planarVelocity.z;
}

void Bumper::OnHurt(HurtArgs args) {
    velocity_ = args.kbVelocity;
    velocity_.y = 0.0f;
}
