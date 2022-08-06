#include "WindowsLayer.h"
#include <sstream>
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
        windowsLayer->ClearMouseMovement();

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        windowsLayer->UpdateMouseMovement();
    }

    return 0;
}