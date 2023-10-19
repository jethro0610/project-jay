#pragma once
#include <glm/vec2.hpp>
#include <FastNoiseLite.h>

const int NOISE_RESOLUTION = 4096;
const int HALF_RESOLUTION = NOISE_RESOLUTION / 2;

enum NoiseAccuracy {
    NA_High,
    NA_Normal,
    NA_Low 
};

class Noise {
public:
    Noise(float range = 1024.0f); 
    Noise(const Noise&) = delete;
    Noise& operator=(const Noise&) = delete;
    ~Noise();

    float Sample(float x, float y, NoiseAccuracy accuracy = NA_Normal);
    float Sample(const glm::vec2& position, NoiseAccuracy accuracy= NA_Normal) { return Sample(position.x, position.y, accuracy); }
    float* GetData() { return (float*)data_; }

private:
    float range_;
    float scale_;
    float data_[NOISE_RESOLUTION][NOISE_RESOLUTION];
    FastNoiseLite fNoise_;
};
