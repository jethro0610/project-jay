#include "TPillar.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"

void TPillar::Init(Entity::InitArgs args) {
    Entity::Init(args);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0] = resourceManager.GetMaterial("m_tpillar");
}
