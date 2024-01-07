#include "ScreenText.h"
#include <assert.h>

ScreenText* ScreenText::screenText_ = nullptr;

ScreenText::ScreenText() {
    assert(screenText_== nullptr);
    screenText_ = this;

    enabled_ = false;
    for (int i = 0; i < MAX_LINES; i++) {
        text_[i].properties_.position.y = i * SCALE;
        text_[i].properties_.scale = SCALE;
    }
}

void ScreenText::SetLine(int line, const std::string& text) {
    screenText_->text_[line] = text;
}

void ScreenText::Clear() {
    for (int i = 0; i < MAX_LINES; i++) {
        screenText_->text_[i] = "";
    }
}

ScreenText* ScreenText::Get() {
    return screenText_;
}

bool ScreenText::IsEnabled() {
    return screenText_->enabled_;
}

void ScreenText::Toggle() {
    screenText_->enabled_ = !screenText_->enabled_;
}

void ScreenText::SetEnabled(bool enabled) {
    screenText_->enabled_ = enabled;
}
