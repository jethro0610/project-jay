#include <cmath>

inline float EaseInOutQuad(float t) {
    if (t < 0.5f)
        return 2.0f * t * t;
    else 
        return 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) * 0.5f;
}
