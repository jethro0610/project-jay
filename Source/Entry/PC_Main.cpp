#include <GLFW/glfw3.h>
#include "Game/Game.h"
#include "Time/Time.h"

#ifdef _DEBUG
#include "../Logging/Logger.h"
#include "../Logging/ScreenText.h"
#endif

GlobalTime globalTime;
#ifdef _DEBUG
Logger logger;
ScreenText screenText;
#endif
Game game;

int main() {
    DEBUGLOG("Starting Project Jay...");
    game.Init();
    DEBUGLOG("Closing Project Jay");
    Logger::Close();
}
