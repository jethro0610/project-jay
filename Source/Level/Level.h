#pragma once
#include "LevelProperties.h"
#include "Editor/EditorTarget.h"
#include "NavPoint.h"
#include <string>
#include <nlohmann/json.hpp>

class EntityList;
class ParticleManager;
class ResourceManager;
class SpreadManager;
class SeedManager;
class Terrain;
class LevelController;

class Level {
public:
    Level(
        EntityList& entities,
        LevelController& levelController,
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
    static constexpr int MAX_NAVPOINTS = 128;
    NavPoint navpoints_[MAX_NAVPOINTS];
    NavPoint& CreateNavpoint(const glm::vec3& position = glm::vec3(0.0f), const std::string& label = "");

    bool DBG_persistView_;
    std::string DBG_name_;
    void SaveGlobals(const std::string& name, const std::string& suffix = "");
    void Save(const std::string& name, const std::string& suffix = "");
    #endif

private:
    EntityList& entities_;
    LevelController& controller_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;
};
