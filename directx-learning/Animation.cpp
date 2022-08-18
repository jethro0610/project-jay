#include "Animation.h"
#include <fstream>
#include "Logger.h"

Animation::Animation(std::string animationName) {
    std::vector<unsigned char> rawBytes;
    std::ifstream infile;

    infile.open(animationName + ".jan", std::ios::in | std::ios::binary);
    assert(infile.is_open());

    // Convert the file stream to a byte vector
    infile.seekg(0, std::ios::end);
    int bytesSize = infile.tellg();
    rawBytes.reserve(bytesSize);
    infile.seekg(0, std::ios::beg);
    std::copy((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>(), std::back_inserter(rawBytes));
    infile.close();

    int jointChannelsCount = (uint8_t)rawBytes.data()[0];
    numOfKeyframes_ = (uint16_t)rawBytes.data()[1];

    jointChannels_.resize(jointChannelsCount);

    for (int i = 0; i < jointChannelsCount; i++) {
        JointChannel* jointChannel = &jointChannels_[i]; 
        jointChannel->keyframes.resize(numOfKeyframes_);
        Transform* keyframeBuffer = (Transform*)&rawBytes.data()[ANIMATION_HEADER_SIZE + KEYFRAME_SIZE * i * numOfKeyframes_];
        for (int k = 0; k < numOfKeyframes_; k++) {
            jointChannel->keyframes[k] = keyframeBuffer[k];
        }
    }
}

Transform Animation::GetJointTransformAtTime(int jointIndex, float time) const {
    int startIndex = ((int)floor(time / ANIMATION_SAMPLE_RATE));
    int endIndex = startIndex + 1;
    if (endIndex >= numOfKeyframes_)
        endIndex = 0;

    float startTime = (startIndex * ANIMATION_SAMPLE_RATE);
    float endTime = startTime + ANIMATION_SAMPLE_RATE; // Since the sample rate is constant, the end will be one sample later

    float lerpTime = (time - startTime) / (endTime - startTime);
    Transform startTransform = jointChannels_[jointIndex].keyframes[startIndex];
    
    Transform endTransform = jointChannels_[jointIndex].keyframes[endIndex];
    Transform lerpTransform = Transform::Lerp(startTransform, endTransform, lerpTime);

    return lerpTransform;
}

float Animation::GetTotalTime() const {
    return numOfKeyframes_ * ANIMATION_SAMPLE_RATE;
}