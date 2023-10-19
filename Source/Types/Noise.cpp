#include "Noise.h"
#include <algorithm>
#include "../Logging/Logger.h"

Noise::Noise(float range):
    range_(range)
{
    scale_ = range_ / NOISE_RESOLUTION;
    for (int x = 0; x < NOISE_RESOLUTION; x++) 
    for (int y = 0; y < NOISE_RESOLUTION; y++)  {
        float sX = (x - HALF_RESOLUTION) * scale_;
        float sY = (y - HALF_RESOLUTION) * scale_;
        data_[y][x] = fNoise_.GetNoise(sX, sY);
    };
}

Noise::~Noise() {

}

float Noise::Sample(float x, float y, NoiseAccuracy accuracy) {
    switch (accuracy) {
        case NA_High: {
            return fNoise_.GetNoise(x, y);
        }

        case NA_Normal: {
            x /= scale_;
            x += HALF_RESOLUTION;
            y /= scale_;
            y += HALF_RESOLUTION;

            int sX = std::clamp((int)x, 0, NOISE_RESOLUTION);
            int sY = std::clamp((int)y, 0, NOISE_RESOLUTION);
            int sX1 = std::min(sX + 1, NOISE_RESOLUTION);
            int sY1 = std::min(sY + 1, NOISE_RESOLUTION);

            float a = x - sX;
            float b = y - sY;

            return 
                (1 - b) * (1 - a) * data_[sY][sX] +
                b * (1 - a) * data_[sY1][sX] +
                (1 - b) * a * data_[sY][sX1] +
                b * a * data_[sY1][sX1];
        }

        case NA_Low: {
            x /= scale_;
            x += HALF_RESOLUTION;
            y /= scale_;
            y += HALF_RESOLUTION;

            int sX = (int)x % NOISE_RESOLUTION;//std::clamp((int)x, 0, NOISE_RESOLUTION);
            int sY = (int)y % NOISE_RESOLUTION;//std::clamp((int)y, 0, NOISE_RESOLUTION);
            return data_[sY][sX];
        }
        
        default:
            return 0.0f;
    }
}
