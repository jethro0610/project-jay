#include "Text.h"
using namespace glm;

Text::Text() {
    properties_.position = vec2(0.0f);
    properties_.scale = 32.0f;
    properties_.kerning= 0.5f;
}

Text::Text(const glm::vec2& position, float scale, float kerning) {
    properties_.position = position;
    properties_.scale = scale;
    properties_.kerning = kerning;
}
