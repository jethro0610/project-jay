#include "ScreenText.h"
using namespace glm;

ScreenText::ScreenText() {
    enabled_ = false;
    for (int i = 0; i < MAX_LINES; i++) 
    for (int j = 0; j < CHARS_PER_LINE; j++) {
        text_[i][j] = vec4(j * 24.0f, i * 48.0f, 48.0f, (float)' ' - 32);
    }
}

void ScreenText::SetLine(int line, std::string text) {
    const int textLength = int(text.size());
    for (int i = 0; i < CHARS_PER_LINE; i++) {
        if (i < textLength)
            screenText_->text_[line][i].w = (float)text[i] - 32;
        else
            screenText_->text_ [line][i].w = (float)' ' - 32;
    }
}
