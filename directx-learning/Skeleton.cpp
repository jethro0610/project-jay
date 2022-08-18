#include "Skeleton.h"
#include <fstream>
#include <assert.h>
#include "Logger.h"
#include <sstream>
Skeleton::Skeleton(std::string skeletonName) {
    std::vector<unsigned char> rawBytes;
    std::ifstream infile;

    infile.open(skeletonName + ".jsk", std::ios::in | std::ios::binary);
    assert(infile.is_open());

    // Convert the file stream to a byte vector
    infile.seekg(0, std::ios::end);
    int bytesSize = infile.tellg();
    rawBytes.reserve(bytesSize);
    infile.seekg(0, std::ios::beg);
    std::copy((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>(), std::back_inserter(rawBytes));
    infile.close();

    int jointCount = (uint8_t)rawBytes.data()[0];

    Joint* jointDataStart = (Joint*)&rawBytes.data()[SKELETON_HEADER_SIZE];
    for (int i = 0; i < jointCount; i++) {
        Joint joint = {};
        joint = jointDataStart[i];

        joints_.push_back(joint);
    }

    testAnim_ = new Animation("sk_test_anim_TestAction");
    testTime_ = 0.0f;
}

void Skeleton::CalculateGlobalJointTransforms(mat4* outputJointTransforms) {
    if (testTime_ >= testAnim_->GetTotalTime())
        testTime_ = 0.0f;

    outputJointTransforms[0] = testAnim_->GetJointTransformAtTime(0, testTime_).GetWorldMatrix();
    CalculateGlobalJointTransforms_Recursive(0, outputJointTransforms);
}

void Skeleton::CalculateGlobalJointTransforms_Recursive(int currentJointIndex, mat4* outputJointTransforms) {
    mat4 parentTransform = outputJointTransforms[currentJointIndex];

    for (int i = 0; i < MAX_JOINT_CHILDREN; i++) {
        int childIndex = joints_[currentJointIndex].childIndices_[i];
        if (childIndex == -1)
            continue;

        mat4 childJointTransform = testAnim_->GetJointTransformAtTime(childIndex, testTime_).GetWorldMatrix();
        outputJointTransforms[childIndex] = parentTransform * childJointTransform;
        CalculateGlobalJointTransforms_Recursive(childIndex, outputJointTransforms);
    }
}