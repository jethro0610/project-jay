#pragma once
#include "Text/Text.h"
#include "Platform/PC_Platform.h"
#include <string>

class EditorTextInput {
public:
    EditorTextInput(Platform& platform);
    void ReadInput();
    void SetLabel(const std::string& label) { label_ = label; }
    void Clear();
    const std::string& Get() { return input_; }
    Text text_;

private:
    Platform& platform_;
    std::string label_;
    std::string input_;
};
