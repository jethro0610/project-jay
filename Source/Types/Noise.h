#pragma once
#include <glm/vec2.hpp>

const int NOISE_RESOLUTION = 1024;
const int HALF_RESOLUTION = NOISE_RESOLUTION / 2;

class Noise {
public:
    Noise(float range = 2048.0f); 
    Noise(const Noise&) = delete;
    Noise& operator=(const Noise&) = delete;
    ~Noise();

    float Sample(float x, float y);
    float Sample(const glm::vec2& position) { return Sample(position.x, position.y ); }
    float SampleFast(float x, float y);
    float SampleFast(const glm::vec2& position) { return SampleFast(position.x, position.y); }
    float* GetData() { return (float*)data_; }

private:
    float range_;
    float data_[NOISE_RESOLUTION][NOISE_RESOLUTION];
};
