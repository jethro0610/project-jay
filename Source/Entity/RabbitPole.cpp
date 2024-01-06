#include "RabbitPole.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"

void RabbitPole::Init(Entity::InitArgs args) {
    Entity::Init(args);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_rabbit_pole");
    materials_[0] = resourceManager.GetMaterial("m_rabbit_pole");
}
