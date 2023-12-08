#include "LevelLoader.h"
#include "../Helpers/LoadHelpers.h"
#include "Entity/EntityManager.h"

LevelLoader::LevelLoader(ResourceManager& resourceManager, EntityManager& entityManager) :
resourceManager_(resourceManager),
entityManager_(entityManager)
{
    
}

void LevelLoader::UnloadLevel() {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entityManager_.entities_[i].alive_)
            continue;

        entityManager_.destroyList_.push_back({i, false});
    }
    entityManager_.DestroyEntities();
}

void LevelLoader::LoadLevel(const std::string& name) {
    UnloadLevel();

    std::ifstream inFile("levels/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load level " + name);

    nlohmann::json data = nlohmann::json::parse(inFile);
    auto& entitiesData = data["entities"];
    Transform entityTransform;
    for (auto& entityData : entitiesData) {
        entityTransform = GetTransform(entityData, "transform");
        entityManager_.spawnList_.push_back({resourceManager_.GetEntityDescription(entityData["name"]), entityTransform});
    }
    entityManager_.SpawnEntities();
}
