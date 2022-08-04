#include "WindowApp.h"

int CALLBACK WinMain(
    HINSTANCE instance,
    HINSTANCE prevInstance,
    LPSTR commandLine,
    int showCode
) {
    WindowApp* windowApp = WindowApp::InitWindowApp(800, 600, "DirectXLearning");

    MSG msg;
    while (windowApp->running_) {
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