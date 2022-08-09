#pragma once
#include <glm.hpp>

struct PerObjectData {
    glm::mat4 modelViewProj;
};

struct ColorVertex {
    glm::vec3 pos;
    glm::vec3 col;
};