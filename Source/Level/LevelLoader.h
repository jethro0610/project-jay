#include "Resource/ResourceManager.h"
#include "Resource/DependencyList.h"
#include "LevelProperties.h"
#include <string>
#include <nlohmann/json.hpp>

class EntityManager;

class LevelLoader {
public:
    LevelLoader(ResourceManager& resourceManager, EntityManager& entityManager);
    void UnloadLevel();
    void LoadLevel(const std::string& name, LevelProperties& outProperties);

private:
    ResourceManager& resourceManager_;
    EntityManager& entityManager_;
    DependencyList GenerateDepedencyList(nlohmann::json& levelData);
};
