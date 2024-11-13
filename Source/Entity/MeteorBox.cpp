#include "MeteorBox.h"
#include "Resource/ResourceManager.h"
using namespace glm;

EntityDependendies MeteorBox::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void MeteorBox::Init(Entity::InitArgs args) {
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_RecieveHits, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void MeteorBox::Update() {
    velocity_.y -= 1.0f; 
}
