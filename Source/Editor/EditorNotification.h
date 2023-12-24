#include <string>
#include "Text/Text.h"

class EditorNotification {
public:
    EditorNotification();
    Text text_;
    void Set(const std::string& notificaiton, float seconds = 1.0f);
    void Clear();
    void Update();

private:
    float timer_;
};
