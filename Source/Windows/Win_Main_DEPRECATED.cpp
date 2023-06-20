#include "../Game/Game.h"
#include "../Game/Time.h"

#ifdef _DEBUG
#include "../Logging/Logger.h"
#include "../Logging/ScreenText.h"
#endif

int CALLBACK WinMain(
    HINSTANCE instance,
    HINSTANCE prevInstance,
    LPSTR commandLine,
    int showCode
) {
#ifdef _DEBUG
    Logger::InitLogger();
    ScreenText::Init();
#endif
    Time::Init();
    Game* app = new Game(1280, 720);
}
