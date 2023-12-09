#pragma once
#include <string>

#ifdef _DEBUG
#define SCREENLINE(line, text) ScreenText::SetLine(line, text)
#else
#define SCREENLINE(text)
#endif

#ifdef _PC
#include <glm/vec4.hpp>
typedef glm::vec4 TextGlyph;
#endif

class ScreenText {
public:
    static constexpr int MAX_LINES = 12;
    static constexpr int CHARS_PER_LINE = 32;

private:
    ScreenText();
    ~ScreenText();
    static ScreenText* screenText_;
    TextGlyph text_[MAX_LINES][CHARS_PER_LINE];
    bool enabled_;

public:
    static ScreenText* Get();
    static bool IsEnabled();
    static void SetLine(int line, const std::string& text);
    static TextGlyph* GetText();
    static void Toggle();
    static void Init();
};
