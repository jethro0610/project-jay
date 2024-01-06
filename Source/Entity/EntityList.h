#pragma once
#include "EntityS.h"
#include <vector_holed.h>

class EntityUnion;
class ParticleManager;
class ResourceManager;

class EntityList {
public:
    EntityList (
        EntityUnion* rawEntities,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        Camera& camera,
        Inputs& inputs,
        Terrain& terrain
    );

    EntityS& operator[](int index);
    EntityS& CreateEntity(EntityS::TypeID type);

private:
    int available_[128];
    int availablePos_;
    
    EntityUnion* rawEntities_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
};
