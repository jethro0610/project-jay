#include "TPillar.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"

void TPillar::Init(Entity::InitArgs args) {
    Entity::Init(args);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(1.0f);
    materials_[0].properties.fresnelBrightness = 0.15f;
}
