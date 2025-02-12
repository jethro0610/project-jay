#include "WorldText.h"
using namespace glm;

WorldText::WorldText() {
    properties_.position = glm::vec3(0.0f);
    properties_.size = 32.0f;
    properties_.hAlignment = Alignment::LEFT;
    properties_.vAlignment = Alignment::TOP;
    for (int i = 0; i < MAX_CHARS; i++) {
        glyphs_[i].index = i;
    }
    length_ = 0;
}

WorldText::WorldText(
    const std::string& str, 
    const glm::vec3& position, 
    float size, 
    float hAlignment, 
    float vAlignment 
) {
    for (int i = 0; i < MAX_CHARS; i++) {
        glyphs_[i].index = i;
    }
    properties_.position = position;
    properties_.size = size;
    properties_.hAlignment = hAlignment;
    properties_.vAlignment = vAlignment;
    SetString(str);
}
