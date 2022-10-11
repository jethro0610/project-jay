#include "SpreadManager.h"
using namespace glm;

SpreadManager::SpreadManager(ResourceManager* resourceManager, World* world) {
    count_ = 0;
    dirty_ = false;
    resourceManager_ = resourceManager;
    world_ = world;
}

void SpreadManager::AddSpread(ivec2 key, float height) {
    if (keys_.contains(key))
        return;

    vec3 position = world_->GetNearestInDirection(vec3(key.x, height, key.y), -Transform::worldUp);

    positions_[count_] = position; 
    keys_[key] = count_;
    count_++;
    dirty_ = true;
}
