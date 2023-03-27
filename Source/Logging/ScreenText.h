#pragma once
#include <integer.hpp>
#include <string>
#include "LogConstants.h"

class ScreenText {
public:
    ScreenText();
    uint32_t lines_[MAX_LINES][CHARS_PER_LINE];
    void SetLine(uint32_t line, std::string text);
    bool dirty_;
};
