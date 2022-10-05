#include "SpreadManager.h"
using namespace glm;

SpreadManager::SpreadManager() {
    count_ = 0;
}
void SpreadManager::AddSpread(ivec2 key) {
    if (keys_.contains(key))
        return;

    Transform spreadTransform;
    spreadTransform.position_ = vec3(0.0f, 48.0f, 0.0f);
    spreadTransform.scale_ = vec3(2.0f);
    keys_[key] = count_;
    transforms_[count_] = spreadTransform;
    count_++;
}
