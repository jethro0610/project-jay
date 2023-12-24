#include "EditorNotification.h"
#include "Time/Time.h"

EditorNotification::EditorNotification() {
    text_ = "";
    timer_ = 0.0f;
}

void EditorNotification::Set(const std::string& notificaiton, float seconds) {
    text_ = notificaiton;
    timer_ = seconds;
}

void EditorNotification::Clear() {
    text_.Clear();
    timer_ = 0.0f;
}

void EditorNotification::Update() {
    if (timer_ > 0.0f)
        timer_ -= GlobalTime::GetDeltaTime();
    else if (text_.length_ != 0) {
        timer_ = 0.0f;
        text_.Clear();
    }
}
