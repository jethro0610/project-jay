#pragma once
#include "Bone.h"
#include <vector>
#include <vector_const.h>

struct Keyframe {
    Pose pose;
};

struct Animation {
    static constexpr int MAX_ANIMATION_NAME = 32;
    static constexpr int MAX_ANIMATIONS = 32;
    static constexpr int KEYFRAMES_PER_SECOND = 60;

    std::vector<Keyframe> keyframes;
    int framerate;
    float speedInfluence;
};

struct AnimationHeader {
    char name[Animation::MAX_ANIMATION_NAME];
    int numKeyframes;
    int framerate;
    float speedInfluence;
};
