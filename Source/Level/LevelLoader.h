#pragma once
#include "Resource/ResourceManager.h"
#include "Resource/DependencyList.h"
#include "LevelProperties.h"
#include "Entity/Entity.h"
#include <string>
#include <nlohmann/json.hpp>
#include <map>

class EntityList;
class ParticleManager;
class SpreadManager;
class SeedManager;
class Terrain;

class LevelLoader {
public:
    LevelLoader(
        EntityList& entities,
        LevelProperties& levelProperties_,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );
    bool LoadLevel(const std::string& name, bool loadTerrain = true);
    void ClearLevel();
    std::map<std::string, Entity::TypeID> entityIds_;

private:
    EntityList& entities_;
    LevelProperties& levelProperties_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;
};
