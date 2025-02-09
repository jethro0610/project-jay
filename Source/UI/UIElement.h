#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct UIElement {
    // vec4 += 2
    glm::vec2 position;
    glm::vec2 size;
    float hAlignment;
    float vAlignment;
    float hAnchor;
    float vAnchor;

    // vec4 += 1
    glm::vec4 color;

    // vec4 += 1
    union {
        float percent;
        glm::vec4 padding;
    };
};
