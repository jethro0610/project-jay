#pragma once
#include "Entity/Entity.h"
#include "LevelProperties.h"
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>

class EntityList;
class ParticleManager;
class ResourceManager;
class SpreadManager;
class SeedManager;
class Terrain;

class Level {
public:
    static constexpr int MAX_PHASES = 4;
    Level(
        EntityList& entities,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );
    bool Load(const std::string& name, const std::string& suffix = "", bool loadTerrain = true);
    void StartPhase();
    void NextPhase();
    void SpawnEntitiesInPhase(int phase, bool persistEntities = false);
    void Clear();
    nlohmann::json phases_[MAX_PHASES];
    LevelProperties properties_;
    bool loaded_;
    int phase_;

    #ifdef _DEBUG
    bool DBG_persistView_;
    std::string DBG_name_;
    std::unordered_map<std::string, Entity::TypeID> DBG_entityTypes_;
    void SaveGlobals(const std::string& name, const std::string& suffix = "");
    void Save(const std::string& name, const std::string& suffix = "");
    void SaveCurrentPhase();
    void EditorSwitchPhase(int phase);
    void TogglePersistView();
    #endif

private:
    EntityList& entities_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;
};
