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

INLINE float EaseCubic(float t) {
    if (t < 0.5f)
        return 4.0f * t * t * t;
    else 
        return 1.0f - POW(-2.0f * t + 2.0f, 3.0f) * 0.5f;
}

INLINE float EaseInOut(float t, float inPower, float outPower) {
    return 1 - POW(1 - POW(t, inPower), outPower);
}

INLINE float EaseOutIn(float t, float inPower, float outPower) {
    return POW(1 - POW(1 - t, outPower), inPower);
}

