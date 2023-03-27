#include "ScreenText.h"

ScreenText::ScreenText() {
    for (int i = 0; i < MAX_LINES; i++) 
    for (int j = 0; j < CHARS_PER_LINE; j++) {
        lines_[i][j] = ' ';
    }
    dirty_ = false;
}

void ScreenText::SetLine(uint32_t line, std::string text) {
    const int textLength = text.size();
    for (int i = 0; i < CHARS_PER_LINE; i++) {
        if (i < textLength)
            lines_[line][i] = text[i];
        else
            lines_[line][i] = ' ';
    }
    dirty_ = true;
}
