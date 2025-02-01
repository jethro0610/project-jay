#include "TPillar.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Terrain/Terrain.h"
#include <glm/gtx/string_cast.hpp>

EntityDependendies TPillar::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void TPillar::Init(Entity::InitArgs args) {
    SetFlag(EF_SendPush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(1.0f);
    materials_[0].properties.fresnelBrightness = 0.15f;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    bubble_ = terrain_->CreateBubble();
    DYN_MOD_RADIUS((*bubble_)) = 120.0f;
    DYN_MOD_VALUE((*bubble_)) = 35.0f;
    DYN_MOD_POS_X((*bubble_)) = transform_.position.x; 
    DYN_MOD_POS_Y((*bubble_)) = transform_.position.z; 
    DYN_MOD_IN_POWER((*bubble_)) = 2.0f;
    DYN_MOD_OUT_POWER((*bubble_)) = 2.0f;
}

void TPillar::Update() {
    transform_.position.x += 0.25f;
    DYN_MOD_POS_X((*bubble_)) = transform_.position.x; 
    DYN_MOD_POS_Y((*bubble_)) = transform_.position.z; 
}

void TPillar::EditorUpdate() {
    DYN_MOD_POS_X((*bubble_)) = transform_.position.x; 
    DYN_MOD_POS_Y((*bubble_)) = transform_.position.z; 
}

void TPillar::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}
