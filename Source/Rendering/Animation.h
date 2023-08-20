#pragma once
#include <vector>
#include "Skeleton.h"
#include "../Types/Transform.h"

const int MAX_ANIMATION_NAME = 32;

struct AnimationHeader {
    char name[MAX_ANIMATION_NAME];
    int numKeyframes;
};

struct Keyframe {
    float time;
    vector_const<Transform, MAX_JOINTS> transform;
};

struct Animation {
    std::vector<Keyframe> keyframes;
};
