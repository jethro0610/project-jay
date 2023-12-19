#include "EditorTextInput.h"
#include <GLFW/glfw3.h>

EditorTextInput::EditorTextInput(Platform& platform):
platform_(platform)
{

}

void EditorTextInput::Clear() {
    input_.clear();
    label_.clear();
    text_.Clear();
}

void EditorTextInput::ReadInput() {
    bool modified = false;
    if (platform_.pressedKeys_[GLFW_KEY_BACKSPACE] && input_.length() > 0) {
        input_.pop_back();
        modified = true;
    }

    for (int i = 0; i < NUM_KEYS; i++) {
        if (i < GLFW_KEY_SPACE || i > 'Z')
            continue;

        if (!platform_.pressedKeys_[i])
            continue;

        modified = true;

        if (
            !platform_.heldKeys_[GLFW_KEY_LEFT_SHIFT] &&
            !platform_.heldKeys_[GLFW_KEY_RIGHT_SHIFT]
        )
            input_ += tolower(i);
        else if (i == '-')
            input_ += 95;
        else
            input_ += i;
    }

    if (modified)
        text_ = label_ + input_;
}
