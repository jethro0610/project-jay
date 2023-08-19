#pragma once
#include <array>
#include <vector>
#include "../Types/Transform.h"
#include "glm/mat4x4.hpp"
const int MAX_JOINTS = 32;
const int MAX_JOINT_CHILDREN = 8;

struct Joint {
    Transform localTransform;
    glm::mat4 inverseBindMatrix;
    std::array<int, MAX_JOINT_CHILDREN> children;
};

struct Skeleton {
    std::vector<Joint> joints;   
};
