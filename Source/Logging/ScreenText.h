#pragma once
#include "Text/Text.h"
#include <string>

#ifdef _DEBUG
#define SCREENLINE(line, text) ScreenText::SetLine(line, text)
#else
#define SCREENLINE(text)
#endif

class ScreenText {
public:
    ScreenText();
    static constexpr int MAX_LINES = 12;
    static constexpr float SCALE = 48.0f;
    static constexpr float KERNING = 0.5f;

private:
    static ScreenText* screenText_;
    std::array<Text, MAX_LINES> text_;
    bool enabled_;

public:
    static ScreenText* Get();
    static bool IsEnabled();
    static void SetLine(int line, const std::string& text);
    static void Clear();
    static std::array<Text, MAX_LINES>& GetText() { return screenText_->text_; }
    static void Toggle();
    static void SetEnabled(bool enabled);
};
