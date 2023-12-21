#pragma once
#include "Resource/ResourceManager.h"
#include "Resource/DependencyList.h"
#include "LevelProperties.h"
#include <string>
#include <nlohmann/json.hpp>

class EntityManager;
class ParticleManager;
class SpreadManager;
class SeedManager;
class Terrain;

class LevelLoader {
public:
    LevelLoader(
        EntityManager& entityManager, 
        LevelProperties& levelProperties_,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );
    void ReloadLevel();
    void LoadLevel(const std::string& name);
    void ClearLevel();

    #ifdef _DEBUG
    std::string DBG_currentLevel;
    #endif 

    DependencyList GenerateEntityDependencyList(const std::string& entityName);

private:
    EntityManager& entityManager_;
    LevelProperties& levelProperties_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;

    DependencyList GenerateDepedencyList(nlohmann::json& levelData);
};
