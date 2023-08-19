#include "SkeletonResource.h"
using namespace glm;

void SkeletonResource::CalculateBasePoseJointTransforms(mat4* outputJointTransforms) const {
    outputJointTransforms[0] = joints_[0].localTransform_.GetWorldMatrix();
    CalculateBasePoseJointTransforms_Recursive(0, outputJointTransforms);
}

void SkeletonResource::CalculateBasePoseJointTransforms_Recursive(int currentJointIndex, mat4* outputJointTransforms) const {
    mat4 parentTransform = outputJointTransforms[currentJointIndex];

    for (int i = 0; i < MAX_JOINT_CHILDREN; i++) {
        int childIndex = joints_[currentJointIndex].childIndices_[i];
        if (childIndex == -1)
            continue;

        mat4 childJointTransform = joints_[childIndex].localTransform_.GetWorldMatrix();
        outputJointTransforms[childIndex] = parentTransform * childJointTransform;
        CalculateBasePoseJointTransforms_Recursive(childIndex, outputJointTransforms);
    }
}
