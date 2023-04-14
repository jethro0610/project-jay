#pragma once
#include "../Types/Transform.h"
#include <vector>

const uint8_t ANIMATION_HEADER_SIZE = sizeof(uint8_t) + sizeof(uint16_t);
const uint8_t ANIMATION_SAMPLE_RATE = uint8_t(1 / 60.0f);
const uint8_t KEYFRAME_SIZE = sizeof(Transform);

struct JointChannel {
    std::vector<Transform> keyframes;
};

class AnimationResource {
public:
    AnimationResource(std::string animationName);
    std::vector<JointChannel> jointChannels_;
    int numOfKeyframes_;

    Transform GetJointTransformAtTime(int jointIndex, float time) const;
    float GetTotalTime() const;
};

