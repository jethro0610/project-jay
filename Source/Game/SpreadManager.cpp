#include "SpreadManager.h"
using namespace glm;

SpreadManager::SpreadManager() {
    count_ = 0;
}
void SpreadManager::AddSpread(ivec2 key) {
    if (keys_.contains(key))
        return;

    Transform spreadTransform;
    spreadTransform.position_ = vec3(key.x, 48.0f, key.y);
    spreadTransform.scale_ = vec3(1.5f);
    keys_[key] = count_;
    transforms_[count_] = spreadTransform;
    count_++;
}
