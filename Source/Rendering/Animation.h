#pragma once
#include <vector>
#include <vector_const.h>
#include "RenderConstants.h"
#include "../Types/Transform.h"

struct AnimationHeader {
    char name[MAX_ANIMATION_NAME];
    int numKeyframes;
};

struct Keyframe {
    vector_const<Transform, MAX_JOINTS> jointTransforms;
};

struct Animation {
    std::vector<Keyframe> keyframes;
};
