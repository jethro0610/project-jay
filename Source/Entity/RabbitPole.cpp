#include "RabbitPole.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"

EntityDependendies RabbitPole::GetStaticDependencies() {
    return {
        "st_rabbit_pole",
        {
            "t_granite_c",
            "t_granite_n"
        }
    };
}

void RabbitPole::Init(Entity::InitArgs args) {
    SetFlag(EF_SendPush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_rabbit_pole");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_variation");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].numTextures = 2;
    materials_[0].textures[0] = resourceManager.GetTexture("t_granite_c");
    materials_[0].textures[1] = resourceManager.GetTexture("t_granite_n");
    materials_[0].variationProperties.fresnelBrightness = 0.25f;
    materials_[0].variationProperties.specularBrightness = 0.15f;
    materials_[0].variationProperties.variationFrequency = 6.0f;
    materials_[0].variationProperties.variationMin = 0.65f;
    materials_[0].variationProperties.variationMax = 1.25f;
    materials_[0].variationProperties.variationPower = 0.5f;
    materials_[0].properties.texScale = glm::vec2(2.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}
