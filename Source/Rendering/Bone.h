#pragma once
#include <glm/mat4x4.hpp>
#include <vector_const.h>
#include "RenderConstants.h"
#include "../Types/Transform.h"

struct Bone {
    glm::mat4 inverseBindMatrix;
    vector_const<int, MAX_BONE_CHILDREN> children;
};
typedef vector_const<Bone, MAX_BONES> Bones;
typedef vector_const<Transform, MAX_BONES> Pose;
typedef vector_const<glm::mat4, MAX_BONES> GPUPose;
