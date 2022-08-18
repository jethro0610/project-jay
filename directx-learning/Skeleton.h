#pragma once
#include <string>
#include <vector>
#include "RawModel.h"
#include "Transform.h"

#include "Animation.h"

#define MAX_JOINT_CHILDREN 8
#define SKELETON_HEADER_SIZE sizeof(uint8_t)
class Joint {
public:
    Transform localTransform_;
    mat4 inverseBindMatrix_;
    int8_t childIndices_[MAX_JOINT_CHILDREN];
};

class Skeleton {
public:
    Skeleton(std::string skeletonName);
    std::vector<Joint> joints_;
    Animation* testAnim_;
    float testTime_;
    void CalculateGlobalJointTransforms(mat4* outputJointTransforms);

private:
    void CalculateGlobalJointTransforms_Recursive(int currentJointIndex, mat4* outputJointTransforms);
};

