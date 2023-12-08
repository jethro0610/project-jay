#pragma once
#include <glm/mat4x4.hpp>
#include <vector_const.h>
#include "SkeletonConstants.h"
#include "../Types/Transform.h"

struct Bone {
    glm::mat4 inverseBindMatrix;
    vector_const<int, SkeletonConstants::MAX_BONE_CHILDREN> children;
};
typedef vector_const<Bone, SkeletonConstants::MAX_BONES> Bones;
typedef vector_const<Transform, SkeletonConstants::MAX_BONES> Pose;
typedef vector_const<glm::mat4, SkeletonConstants::MAX_BONES> GPUPose;
