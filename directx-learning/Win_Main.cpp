#include "WindowsLayer.h"

int CALLBACK WinMain(
    HINSTANCE instance,
    HINSTANCE prevInstance,
    LPSTR commandLine,
    int showCode
) {
    WindowsLayer* windowsLayer = WindowsLayer::InitWindowsLayer(800, 600, "DirectXLearning");

    MSG msg;
    while (!windowsLayer->closed_) {
        windowsLayer->ClearPressedAndReleasedKeys();
        if (GetMessage(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            break; // This shouldn't happen, log here
        }
    }

    return 0;
}