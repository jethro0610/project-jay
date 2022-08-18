#include "AnimationResource.h"

Transform AnimationResource::GetJointTransformAtTime(int jointIndex, float time) const {
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

float AnimationResource::GetTotalTime() const {
    return numOfKeyframes_ * ANIMATION_SAMPLE_RATE;
}