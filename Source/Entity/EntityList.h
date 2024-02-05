#pragma once
#include "Entity.h"
#include <vector_holed.h>

struct EntityUnion;
class ParticleManager;
class ResourceManager;

class EntityList {
public:
    EntityList(
        EntityUnion* rawEntities,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        Camera& camera,
        Inputs& inputs,
        Level& level,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );

    Entity& operator[](int index);
    Entity& CreateEntity(Entity::TypeID type, const Transform& transform = Transform());
    void DestroyFlaggedEntities();
    void Reset();
    bool IsAnyOverlapping(Entity& entity);

private:
    int available_[128];
    int availablePos_;
    
    EntityUnion* rawEntities_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
};
