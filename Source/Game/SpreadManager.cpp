#include "SpreadManager.h"
using namespace glm;

void SpreadManager::AddSpread(ivec2 key) {
    if (keys_.contains(key))
        return;

    positions_[count_] = vec3(key.x, 48.0f, key.y);
    keys_[key] = count_;
    count_++;
    dirty_ = true;
}
