#pragma once
#include <glm.hpp>

struct PerObjectData {
    glm::mat4 worldViewProj;
};

struct ColorVertex {
    glm::vec3 pos;
    glm::vec3 col;
};