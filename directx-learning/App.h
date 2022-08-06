#pragma once

#ifdef WINDOWS
#include "WindowsLayer.h"
#endif

class App {
public:
    App();
    void Run();

    float forwardInput_;
    float sideInput_;

    #ifdef WINDOWS
    bool running_;
    #endif

private:
    #ifdef WINDOWS
    WindowsLayer* windowsLayer_;
    #endif

    void UpdateInputs();
};