#pragma once
struct NoiseLayer {
    static constexpr int MAX = 4;
    bool active;
    int seed;
    float frequency;
    float multiplier;
    float exponent;

    NoiseLayer() {
        active = false;
        seed = 1337;
        frequency = 0.15f;
        multiplier = 12.0f;
        exponent = 1.0f;
    }
};
