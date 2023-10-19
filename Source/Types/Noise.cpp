#define _USE_MATH_DEFINES
#include "Noise.h"
#include <FastNoiseLite.h>
#include <cmath>
#include "../Logging/Logger.h"

Noise::Noise(float range) {
    range_ = range;
    FastNoiseLite fNoise;

    for (int x = 0; x < NOISE_RESOLUTION; x++) 
    for (int y = 0; y < NOISE_RESOLUTION; y++)  {
        float sX = (((float)x / NOISE_RESOLUTION)) * range_;
        float sY = (((float)y / NOISE_RESOLUTION)) * range_;
        data_[x][y] = 0.5f;
    };
}

Noise::~Noise() {

}

float Noise::Sample(float x, float y) {
    int fx = int(floor(x)) % NOISE_RESOLUTION;
    int fx1 = (fx + 1) % NOISE_RESOLUTION;
    int fy = int(floor(y)) % NOISE_RESOLUTION;
    int fy1 = (fy + 1) % NOISE_RESOLUTION;
    float a = x - fx;
    float b = y - fy;

    float val =
        (1 - a) * (1 - b) * data_[fx][fy] +
        a * (1 - b) * data_[fx1][fy] +
        (1 - a) * b * data_[fx][fy1] +
        a * b * data_[fx1][fy1];

    return val;
}

float Noise::SampleFast(float x, float y) {
    int x0 = int(floor(x)) % NOISE_RESOLUTION;
    int y0 = int(floor(y)) % NOISE_RESOLUTION;
    return data_[x0][y0];
}
