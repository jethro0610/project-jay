#include "SpreadManager.h"
using namespace glm;

void SpreadManager::AddSpread(ivec2 key, float height, World* world) {
    if (keys_.contains(key))
        return;

    vec3 position = world->GetNearestInDirection(vec3(key.x, height, key.y), -Transform::worldUp);

    positions_[count_] = position; 
    keys_[key] = count_;
    count_++;
    dirty_ = true;
}
