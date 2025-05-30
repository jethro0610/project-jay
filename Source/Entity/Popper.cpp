#include "Popper.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"
using namespace glm;

EntityDependendies Popper::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Popper::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_speed", &speed_},
            {"p_height", &fader_.maxs_[0]},
            {"p_radius", &bubble_->radius},
            {"p_deflate", &fader_.deactivateLength_},
        },
        {
            // Ints
            {"p_maxpop", &maxPopTime_}
        },
        {
            // Bools
        }
    };
}

void Popper::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(1.0f, 1.0f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.25f;

    bubble_ = terrain_->CreateBubble();
    bubble_->radius = 100.0f;
    bubble_->inpower = 2.0f;
    bubble_->outpower = 1.75f;
    fader_ = DynamicFader();
    fader_.activateLength_ = 30.0f;
    fader_.deactivateLength_ = 60.0f * 2.0f;
    fader_.AddModifier(bubble_, &bubble_->height, 0.0f, 50.0f, false, EaseType::E_Elastic);
    speed_ = 50.0f;

    popTime_ = 0;
    maxPopTime_ = 120;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_SendPush, true);
    traceDistance_ = 1000.0f;
}

void Popper::Start() {
    fader_.DeactivateModifiers(true);
    targetDistFromEdge_ = -terrain_->GetDistance(transform_.position).x;
}

void Popper::Update() {
    fader_.Update();
    bubble_->position = transform_.position;

    vec2 terrainDist = terrain_->GetDistance(transform_.position);
    transform_.position.y = terrainDist.y;
    float distFromEdge = -terrainDist.x;
    vec3 directionToEdge = terrain_->GetDirectionToEdge(transform_.position);

    transform_.position += vec3(directionToEdge.z, 0.0f, -directionToEdge.x) * speed_ * GlobalTime::TIMESTEP;

    float distScalar = targetDistFromEdge_ - distFromEdge;
    transform_.position += distScalar * 0.15f * directionToEdge * GlobalTime::TIMESTEP; 

    if (fader_.IsActive()) {
        popTime_--;
        if (popTime_ <= 0) {
            fader_.StartDeactivate(); 
        }
    }
}

void Popper::OnHurt(HurtArgs args) {
    fader_.StartActivate();
    popTime_ = maxPopTime_;
}

void Popper::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}
