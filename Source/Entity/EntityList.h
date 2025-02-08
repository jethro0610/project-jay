#pragma once
#include "Entity/TypeID.h"
#include "Entity.h"
#include "Types/Transform.h"
#include <vector_holed.h>

class Camera;
struct Inputs;
class Level;
class SeedManager;
class SpreadManager;
class Terrain;
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
    Entity& CreateEntity(TypeID type, const Transform& transform = Transform(), bool skipStart = false);
    void DestroyFlaggedEntities();
    void Reset();
    bool IsAnyOverlapping(Entity& entity);

private:
    int available_[Entity::MAX];
    int availablePos_;
    
    EntityUnion* rawEntities_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
};
