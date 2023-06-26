#include <GLFW/glfw3.h>
#include "../Game/Game.h"
#include "../Game/Time.h"

#ifdef _DEBUG
#include "../Logging/Logger.h"
#include "../Logging/ScreenText.h"
#endif

int main() {
#ifdef _DEBUG
    Logger::Init();
    ScreenText::Init();
#endif
    Time::Init();
    Game* game = new Game();
    Logger::Close();
}
