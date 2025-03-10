#include <GLFW/glfw3.h>
#include "Game/Game.h"
#include "Time/Time.h"
#include <FastNoiseLite.h>

#ifdef _DEBUG
#include "../Logging/Logger.h"
#include "../Logging/ScreenText.h"
#endif

GlobalTime globalTime;
#ifdef _DEBUG
Logger logger;
ScreenText screenText;
#endif

int main() {
    DEBUGLOG("Starting Project Jay...");
    DEBUGLOG("Entity union size: " << sizeof(EntityUnion) << "B");
    DEBUGLOG("Total entity size: " << (sizeof(EntityUnion) * EntityList::MAX) / 1024 / 1024 << "MB");
    // No clue why but this new call prevents vertices from uninitializing
    // looking into this...
    Game* game = new Game(); 
    game->Init();
    DEBUGLOG("Closing Project Jay");
    Logger::Close();
}
