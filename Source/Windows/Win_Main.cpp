#include "../Game/Game.h"
#include "../Logging/Logger.h"

int CALLBACK WinMain(
    HINSTANCE instance,
    HINSTANCE prevInstance,
    LPSTR commandLine,
    int showCode
) {
#ifdef _DEBUG
    Logger::InitLogger();
#endif
    Game* app = new Game(1280, 720);
}
