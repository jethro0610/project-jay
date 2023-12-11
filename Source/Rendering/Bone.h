#pragma once
#include "Types/Transform.h"
#include <glm/mat4x4.hpp>
#include <vector_const.h>

struct Bone {
    static constexpr int MAX_BONE_CHILDREN = 8;
    static constexpr int MAX_BONES = 64;
    Bone() {
        inverseBindMatrix = {};
        children = {};
    }
    glm::mat4 inverseBindMatrix;
    vector_const<int, MAX_BONE_CHILDREN> children;
};

typedef vector_const<Bone, Bone::MAX_BONES> Bones;
typedef vector_const<Transform, Bone::MAX_BONES> Pose;
typedef vector_const<glm::mat4, Bone::MAX_BONES> GPUPose;
