#ifndef SHARED_SHADER
#pragma once
#define INLINE inline
#include <cmath>
#define POW powf
#else
#define INLINE
#define POW pow
#endif

INLINE float EaseQuad(float t) {
    if (t < 0.5f)
        return 2.0f * t * t;
    else 
        return 1.0f - POW(-2.0f * t + 2.0f, 2.0f) * 0.5f;
}

INLINE float EaseInQuad(float t) {
    return t * t;
}

INLINE float EaseOutQuad(float t) {
    float inv = 1.0f - t;
    return 1.0f - (inv * inv * inv);
}

INLINE float EaseCubic(float t) {
    if (t < 0.5f)
        return 4.0f * t * t * t;
    else 
        return 1.0f - POW(-2.0f * t + 2.0f, 3.0f) * 0.5f;
}

INLINE float EaseInCubic(float t) {
    return t * t * t;
}

INLINE float EaseOutCubic(float t) {
    float inv = 1.0f - t;
    return 1.0f - (inv * inv * inv);
}

INLINE float EaseInOut(float t, float inPower, float outPower) {
    return 1 - POW(1 - POW(t, inPower), outPower);
}

INLINE float EaseOutIn(float t, float inPower, float outPower) {
    return POW(1 - POW(1 - t, outPower), inPower);
}

#ifndef SHARED_SHADER
INLINE float EaseElastic(float t) {
    constexpr float C5 = (2 * M_PI) / 4.5;

    return t == 0.0f
      ? 0.0f
      : t == 1.0f
      ? 1.0f
      : t < 0.5f
      ? -(powf(2.0f, 20.0f * t - 10.0f) * sinf((20.0f * t - 11.125f) * C5)) / 2.0f
      : (powf(2.0f, -20.0f * t + 10.0f) * sinf((20.0f * t - 11.125f) * C5)) / 2.0f + 1.0f;
}

INLINE float EaseInElastic(float t) {
    constexpr float C4 = (2.0f * M_PI) / 3.0f;

    return t == 0.0f
      ? 0.0f
      : t == 1.0f
      ? 1.0f
      : -powf(2.0f, 10.0f * t - 10.0f) * sinf((t * 10.0f - 10.75f) * C4);
}

INLINE float EaseOutElastic(float t) {
    constexpr float C4 = (2.0f * M_PI) / 3.0f;

    return t == 0.0f
      ? 0.0f
      : t == 1.0f
      ? 1.0f
      : powf(2.0f, -10.0f * t) * sinf((t * 10.0f - 0.75f) * C4) + 1.0f;
}
#endif


#ifndef SHARED_SHADER
#include "EaseType.h"
INLINE float Ease(float t, EaseType type) {
    switch (type) {
        case E_Linear:
            return t;

        case E_Quad:
            return EaseQuad(t);

        case E_InQuad:
            return EaseInQuad(t);

        case E_OutQuad:
            return EaseOutQuad(t);

        case E_Cubic:
            return EaseCubic(t);

        case E_InCubic:
            return EaseInCubic(t);

        case E_OutCubic:
            return EaseOutCubic(t);

        case E_Elastic:
            return EaseElastic(t);

        case E_InElastic:
            return EaseInElastic(t);
        
        case E_OutElastic:
            return EaseOutElastic(t);
    }
    return t;
}
#endif
