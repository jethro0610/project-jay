#pragma once
#include <vector>
#include <vector_const.h>
#include "Bone.h"
#include "RenderConstants.h"
#include "../Types/Transform.h"

struct AnimationHeader {
    char name[MAX_ANIMATION_NAME];
    int numKeyframes;
};

struct Keyframe {
    Pose pose;
};

struct Animation {
    std::vector<Keyframe> keyframes;
};
