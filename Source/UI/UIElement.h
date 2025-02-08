#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct UIElement {
    static constexpr float LEFT_ALIGN = 0.0f;
    static constexpr float RIGHT_ALIGN = 1.0f;
    static constexpr float TOP_ALIGN = 0.0f;
    static constexpr float BOTTOM_ALIGN = 1.0f;
    static constexpr float CENTER_ALIGN = 0.5f;

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
