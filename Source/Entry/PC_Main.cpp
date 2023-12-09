#include <GLFW/glfw3.h>
#include "Game/Game.h"
#include "Time/Time.h"

#ifdef _DEBUG
#include "../Logging/Logger.h"
#include "../Logging/ScreenText.h"
#endif

int main() {
#ifdef _DEBUG
    Logger::Init();
    ScreenText::Init();
#endif
    GlobalTime::Init();
    DEBUGLOG("Starting Project Jay...");
    Game* game = new Game();
    DEBUGLOG("Closing Project Jay");
    Logger::Close();
}
