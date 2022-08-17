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

        if (i == 2)
            joint.localTransform_.position_.z += 1.0f;

        joints_.push_back(joint);
    }
}

void Skeleton::CalculateGlobalJointTransforms(mat4* outputJointTransforms) {
    outputJointTransforms[0] = joints_[0].localTransform_.GetWorldMatrix();
    CalculateGlobalJointTransforms_Recursive(0, outputJointTransforms);
}

void Skeleton::CalculateGlobalJointTransforms_Recursive(int currentJointIndex, mat4* outputJointTransforms) {
    mat4 parentTransform = outputJointTransforms[currentJointIndex];
    for (int i = 0; i < MAX_JOINT_CHILDREN; i++) {
        int childIndex = joints_[currentJointIndex].childIndices_[i];
        if (childIndex == -1)
            continue;

        mat4 childJointTransform = joints_[childIndex].localTransform_.GetWorldMatrix();
        outputJointTransforms[childIndex] = parentTransform * childJointTransform;
        CalculateGlobalJointTransforms_Recursive(childIndex, outputJointTransforms);
    }
}