#pragma once

#ifdef WINDOWS
#include "WindowsLayer.h"
#include "XInputHandler.h"
#endif

class App {
public:
    App();

    bool running_;
    void Run();

    float forwardInput_;
    float sideInput_;

private:
    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    XInputHandler inputHandler;
    #endif

    void UpdateInputs();
};