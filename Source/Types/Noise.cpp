#include "Noise.h"
#include <algorithm>
#include <FastNoiseLite.h>
#include "../Logging/Logger.h"

Noise::Noise(float range):
    range_(range)
{
    FastNoiseLite fNoise;

    for (int x = 0; x < NOISE_RESOLUTION; x++) 
    for (int y = 0; y < NOISE_RESOLUTION; y++)  {
        float sX = (((float)(x + HALF_RESOLUTION) / NOISE_RESOLUTION)) * range_;
        float sY = (((float)(y + HALF_RESOLUTION) / NOISE_RESOLUTION)) * range_;
        data_[y][x] = fNoise.GetNoise(sX, sY);
    };
}

Noise::~Noise() {

}

float Noise::Sample(float x, float y) {
    x /= range_;
    x *= NOISE_RESOLUTION;
    x += HALF_RESOLUTION;
    y /= range_;
    y *= NOISE_RESOLUTION;
    y += HALF_RESOLUTION;

    int fx = std::clamp((int)x, 0, NOISE_RESOLUTION);
    int fx1 = std::min(fx + 1, NOISE_RESOLUTION);

    int fy = std::clamp((int)y, 0, NOISE_RESOLUTION);
    int fy1 = std::min(fy + 1, NOISE_RESOLUTION);

    float a = x - fx;
    float b = y - fy;

    float val =
        (1 - b) * (1 - a) * data_[fy][fx] +
        b * (1 - a) * data_[fy1][fx] +
        (1 - b) * a * data_[fy][fx1] +
        b * a * data_[fy1][fx1];

    return val;
}

float Noise::SampleFast(float x, float y) {
    x /= range_;
    x *= NOISE_RESOLUTION;
    x += HALF_RESOLUTION;
    y /= range_;
    y *= NOISE_RESOLUTION;
    y += HALF_RESOLUTION;

    int fx = std::clamp((int)x, 0, NOISE_RESOLUTION);
    int fy = std::clamp((int)y, 0, NOISE_RESOLUTION);
    return data_[fy][fx];
}
