#pragma once
#include "Entity/Entity.h"
#include "LevelProperties.h"
#include <string>
#include <nlohmann/json.hpp>
#include <map>

class EntityList;
class ParticleManager;
class ResourceManager;
class SpreadManager;
class SeedManager;
class Terrain;

class Level {
public:
    Level(
        EntityList& entities,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );
    bool Load(const std::string& name, const std::string& suffix = "", bool loadTerrain = true);
    void Clear();
    LevelProperties properties_;
    bool loaded_;

    #ifdef _DEBUG
    std::string DBG_name_;
    void Save(const std::string& name, const std::string& suffix = "");
    #endif

private:
    std::map<std::string, Entity::TypeID> entityIds_;
    EntityList& entities_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;
};
