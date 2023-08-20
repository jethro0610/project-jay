#pragma once
#include <array>
#include <vector>
#include <vector_const.h>
#include "../Types/Transform.h"
#include "glm/mat4x4.hpp"
const int MAX_JOINTS = 32;
const int MAX_JOINT_CHILDREN = 8;

struct Joint {
    Transform localTransform;
    glm::mat4 inverseBindMatrix;
    vector_const<int, MAX_JOINT_CHILDREN> children;
};

struct Skeleton {
    vector_const<Joint, MAX_JOINTS> joints;   
};
