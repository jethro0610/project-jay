#include "GlobalNoise.h"
#include <assert.h>
GlobalNoise* GlobalNoise::instance_ = nullptr;

GlobalNoise::GlobalNoise() {
    noise_ = FastNoiseLite(5354);
}

void GlobalNoise::Init() {
    assert(instance_ == nullptr);
    instance_ = new GlobalNoise();
}

float GlobalNoise::Sample(float x, float y) {
    return instance_->noise_.GetNoise(x, y);
}

float GlobalNoise::Sample(const glm::vec2& pos) {
    return instance_->noise_.GetNoise(pos.x, pos.y);
}

float GlobalNoise::Sample(const glm::vec3& pos) {
    return instance_->noise_.GetNoise(pos.x, pos.z);
}
