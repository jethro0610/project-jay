#include "../Game.h"
#include "../Logger.h"

int CALLBACK WinMain(
    HINSTANCE instance,
    HINSTANCE prevInstance,
    LPSTR commandLine,
    int showCode
) {
#ifdef _DEBUG
    Logger::InitLogger();
#endif
    Game app;
    app.PlatformInit();
}