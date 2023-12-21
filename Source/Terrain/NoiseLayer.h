#pragma once
#include <glm/vec2.hpp>
struct NoiseLayer {
    static constexpr int MAX = 4;
    bool active;
    int seed;
    glm::vec2 frequency;
    float multiplier;
    float exponent;

    NoiseLayer() {
        active = false;
        seed = 1337;
        frequency.x = 0.15f;
        frequency.y = 0.15f;
        multiplier = 12.0f;
        exponent = 1.0f;
    }
};
