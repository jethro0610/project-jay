#pragma once
#include <string>
#include "LogConstants.h"

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
private:
    ScreenText();
    ~ScreenText();
    static ScreenText* screenText_;
    TextGlyph text_[MAX_LINES][CHARS_PER_LINE];
    bool enabled_;

public:
    static ScreenText* Get();
    static bool IsEnabled();
    static void SetLine(uint32_t line, std::string text);
    static TextGlyph* GetText();
    static void Toggle();
    static void Init();
};
