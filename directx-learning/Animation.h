#pragma once
#include "Transform.h"
#include <vector>

#define ANIMATION_HEADER_SIZE (sizeof(uint8_t) + sizeof(uint16_t))
#define ANIMATION_SAMPLE_RATE (1 / 60.0f)
#define KEYFRAME_SIZE sizeof(Transform)

struct JointChannel {
    std::vector<Transform> keyframes;
};

class Animation {
public:
    Animation(std::string animationName);
    std::vector<JointChannel> jointChannels_;
    int numOfKeyframes_;

    Transform GetJointTransformAtTime(int jointIndex, float time) const;
    float GetTotalTime() const;
};

