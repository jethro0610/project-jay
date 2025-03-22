#include "SnakeSegment.h"
#include "Resource/ResourceManager.h"
#include "SnakeHead.h"
using namespace glm;

EntityDependendies SnakeSegment::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void SnakeSegment::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
    traceDistance_ = 100.0f;

    SetFlag(EF_Interpolate, true);
    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecievePush, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_StickToGround, true);
}

void SnakeSegment::PreUpdate() {
    hitlag_ = head_->hitlag_;
}

void SnakeSegment::Update() { 
    vec3 fromPrevToSelf = transform_.position - prevSegment_->transform_.position;
    fromPrevToSelf.y = 0.0f;
    float distTargetToSelf = length(fromPrevToSelf);
    if (distTargetToSelf  > distFromPrev_) {
        fromPrevToSelf /= distTargetToSelf;
        fromPrevToSelf *= distFromPrev_;
        transform_.position = prevSegment_->transform_.position + fromPrevToSelf;
    }
}
