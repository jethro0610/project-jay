#include "StarBalloon.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Time/Time.h"
#include "Helpers/Random.h"
using namespace glm;

EntityDependendies StarBalloon::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void StarBalloon::Init(Entity::InitArgs args) {
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_Overlap, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 2.0f;
    overlapbox_.bottom = 2.0f;
    overlapbox_.radius = 2.0f;
    timeOffset_ = RandomFloatRange(0.0f, 60.0f);
}

void StarBalloon::Start() {
    originalY_ = transform_.position.y;
}

void StarBalloon::Update() {
    transform_.position.y = originalY_ + sin(GlobalTime::GetTime() * 1.5f + timeOffset_) * 3.0f;
}

void StarBalloon::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    overlappedEntity->hitlag_ = 4;
    hitlag_ = 4;
    stun_ = true;
}

void StarBalloon::OnHitlagEnd() {
    destroy_ = true;
}
