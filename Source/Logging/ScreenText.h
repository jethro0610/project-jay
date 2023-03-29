#pragma once
#include <integer.hpp>
#include <string>
#include "LogConstants.h"

#ifdef _DEBUG
#define SCREENLINE(line, text) ScreenText::SetLine(line, text)
#else
#define SCREENLINE(text)
#endif

class ScreenText {
private:
    ScreenText();
    ~ScreenText();
    static ScreenText* screenText_;
    uint32_t lines_[MAX_LINES][CHARS_PER_LINE];
    bool enabled_;

public:
    static ScreenText* Get();
    static bool IsEnabled();
    static void SetLine(uint32_t line, std::string text);
    static uint32_t* GetLines();
    static void Toggle();
    static void Init();
};
