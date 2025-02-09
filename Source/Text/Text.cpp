#include "Text.h"
using namespace glm;

Text::Text() {
    properties_.position = vec2(0.0f);
    properties_.scale = 32.0f;
    properties_.kerning = 0.5f;
    properties_.hAlignment = Alignment::LEFT;
    properties_.vAlignment = Alignment::TOP;
    properties_.hAnchor = Alignment::LEFT;
    properties_.vAnchor = Alignment::TOP;
    for (int i = 0; i < MAX_CHARS; i++) {
        glyphs_[i].index = i;
    }
    length_ = 0;
}

Text::Text(const glm::vec2& position, float scale, float kerning) {
    properties_.position = position;
    properties_.scale = scale;
    properties_.kerning = kerning;
}
