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

class LevelLoader {
public:
    LevelLoader(
        EntityManager& entityManager, 
        LevelProperties& levelProperties_,
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager
    );
    void LoadLevel(const std::string& name);
    void ClearLevel();

    #ifdef _DEBUG
    std::string DBG_currentLevel;
    #endif 

private:
    EntityManager& entityManager_;
    LevelProperties& levelProperties_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    DependencyList GenerateDepedencyList(nlohmann::json& levelData);
};
