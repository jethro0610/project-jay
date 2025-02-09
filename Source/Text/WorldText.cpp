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
