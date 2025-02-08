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
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.fresnelBrightness = 0.15f;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

