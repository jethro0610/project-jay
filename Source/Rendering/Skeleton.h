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

// Internal is defined here so we can store the actual
// values in the renderer. Everything else will use a
// pointer to that skeleton. This follows the same semantics
// as bgfx's handles
struct Skeleton_INTERNAL {
    std::vector<Joint> joints;   
};

typedef Skeleton_INTERNAL* Skeleton;
