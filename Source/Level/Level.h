#pragma once
#include "LevelProperties.h"
#include <string>
#include <nlohmann/json.hpp>

class Currency;
class EntityList;
class ParticleManager;
class ResourceManager;
class SpreadManager;
class SeedManager;
class Terrain;

class Level {
public:
    Level(
        Currency& currency,
        EntityList& entities,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );
    bool Load(const std::string& name, const std::string& suffix = "", bool loadTerrain = true, bool editorLoad = false);
    void Clear(bool clearTerrain = false);
    LevelProperties properties_;
    bool loaded_;

    #ifdef _DEBUG
    bool DBG_persistView_;
    std::string DBG_name_;
    void SaveGlobals(const std::string& name, const std::string& suffix = "");
    void Save(const std::string& name, const std::string& suffix = "");
    #endif

private:
    Currency& currency_;
    EntityList& entities_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;
};
