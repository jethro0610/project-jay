#include "RabbitPole.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"

void RabbitPole::Init(Entity::InitArgs args) {
    Entity::Init(args);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_rabbit_pole");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_variation");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].numTextures = 2;
    materials_[0].textures[0] = resourceManager.GetTexture("t_granite_c");
    materials_[0].textures[1] = resourceManager.GetTexture("t_granite_n");
    materials_[0].properties[MPROP_FRES_BRIGHTNESS] = 0.25f;
    materials_[0].properties[MPROP_SPEC_BRIGHTNESS] = 0.15f;
    materials_[0].properties[MPROP_VARIATION_FREQUENCY] = 6.0f;
    materials_[0].properties[MPROP_VARIATION_MIN] = 0.65f;
    materials_[0].properties[MPROP_VARIATION_MAX] = 1.25f;
    materials_[0].properties[MPROP_VARIATION_POWER] = 0.5f;
    materials_[0].properties[MPROP_TEXSCALE_X] = 2.0f;
    materials_[0].properties[MPROP_TEXSCALE_Y] = 2.0f;
}
