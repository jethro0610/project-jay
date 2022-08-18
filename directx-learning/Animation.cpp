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
    int keyframeCount = (uint16_t)rawBytes.data()[1];

    jointChannels_.resize(jointChannelsCount);

    for (int i = 0; i < jointChannelsCount; i++) {
        JointChannel* jointChannel = &jointChannels_[i]; 
        jointChannel->keyframes.resize(keyframeCount);
        Transform* keyframeBuffer = (Transform*)&rawBytes.data()[ANIMATION_HEADER_SIZE + KEYFRAME_SIZE * i * keyframeCount];
        for (int k = 0; k < keyframeCount; k++) {
            jointChannel->keyframes[k] = keyframeBuffer[k];
        }
    }
}

Transform Animation::GetJointTransformAtTime(int jointIndex, float time) {
    int index = ((int)floor(time / ANIMATION_SAMPLE_RATE)) % jointChannels_[jointIndex].keyframes.size();
    return jointChannels_[jointIndex].keyframes[index];
}