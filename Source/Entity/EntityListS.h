#pragma once
#include "EntityS.h"
#include <vector_holed.h>

class EntityUnion;
class ParticleManager;
class ResourceManager;

class EntityListS {
public:
    EntityListS (
        EntityUnion* rawEntities,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        Camera& camera,
        Inputs& inputs,
        Terrain& terrain
    );

    EntityS& operator[](int index);

    void CreateEntity(EntityS::TypeID type);
    void DestroyEntity(int entityId);
    bool Valid(int entityId) { return available_[entityId] == -1; }

private:
    int available_[128];
    int availablePos_;
    
    EntityUnion* rawEntities_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
};
