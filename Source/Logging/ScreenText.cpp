#include "ScreenText.h"

ScreenText* ScreenText::screenText_ = nullptr;

ScreenText::ScreenText() {
    enabled_ = false;
    for (int i = 0; i < MAX_LINES; i++) 
    for (int j = 0; j < CHARS_PER_LINE; j++) {
        lines_[i][j] = ' ' - 32;
    }
}

ScreenText* ScreenText::Get() {
    return screenText_;
}

bool ScreenText::IsEnabled() {
    return screenText_->enabled_;
}

void ScreenText::SetLine(uint32_t line, std::string text) {
    const int textLength = text.size();
    for (int i = 0; i < CHARS_PER_LINE; i++) {
        if (i < textLength)
            screenText_->lines_[line][i] = text[i] - 32;
        else
            screenText_->lines_[line][i] = ' ' - 32;
    }
}

uint32_t* ScreenText::GetLines() {
    return screenText_->lines_[0];
}

void ScreenText::Toggle() {
    screenText_->enabled_ = !screenText_->enabled_;
}

void ScreenText::Init() {
    assert(screenText_== nullptr);
    screenText_ = new ScreenText();
}
