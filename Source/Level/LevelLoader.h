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
        ParticleManager& particleManager,
        ResourceManager& resourceManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager
    );
    void LoadLevel(const std::string& name, LevelProperties& outProperties);
    void ClearLevel();
    void LoadLevel();

private:
    EntityManager& entityManager_;
    ParticleManager& particleManager_;
    ResourceManager& resourceManager_;
    SeedManager& seedManager_;
    SpreadManager& spreadManager_;
    DependencyList GenerateDepedencyList(nlohmann::json& levelData);
    nlohmann::json levelData_;
};
