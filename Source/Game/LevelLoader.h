#include <string>
#include <nlohmann/json.hpp>
#include "ResourceManager.h"

class EntityManager;

class LevelLoader {
public:
    LevelLoader(ResourceManager& resourceManager, EntityManager& entityManager);
    void UnloadLevel();
    void LoadLevel(const std::string& name);

private:
    ResourceManager& resourceManager_;
    EntityManager& entityManager_;
};
