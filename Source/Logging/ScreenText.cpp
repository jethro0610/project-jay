#include <assert.h>
#include "ScreenText.h"

ScreenText* ScreenText::screenText_ = nullptr;

ScreenText* ScreenText::Get() {
    return screenText_;
}

bool ScreenText::IsEnabled() {
    return screenText_->enabled_;
}

TextGlyph* ScreenText::GetText() {
    return screenText_->text_[0];
}

void ScreenText::Toggle() {
    screenText_->enabled_ = !screenText_->enabled_;
}

void ScreenText::Init() {
    assert(screenText_== nullptr);
    screenText_ = new ScreenText();
}
