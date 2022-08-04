#include "WindowApp.h"
#include <stdexcept>

WindowApp* WindowApp::windowApp_ = nullptr;

WindowApp::WindowApp(int width, int height, const char* name) {
    windowApp_ = this;
    running_ = true;

    // Create and register the Window class
    WNDCLASS windowClass = {};
    windowClass.style;
    windowClass.lpfnWndProc = HandleMsgThunk;
    classInstance_ = GetModuleHandle(nullptr);
    windowClass.hInstance = classInstance_;
    windowClass.lpszClassName = className_;
    windowClass.style = CS_OWNDC;
    RegisterClass(&windowClass);

    // Calculate the window size
    RECT wRect = {};
    wRect.right = width;
    wRect.bottom = height;
    AdjustWindowRect(&wRect, WS_CAPTION | WS_MINIMIZEBOX, FALSE);

    // Create the window and its handle
    windowHandle_ = CreateWindow(
        className_,
        name,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wRect.right,
        wRect.bottom,
        nullptr,
        nullptr,
        classInstance_,
        this
    );

    ShowWindow(windowHandle_, SW_SHOWDEFAULT);
}

LRESULT CALLBACK WindowApp::HandleMsgThunk(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam) {
    return GetWindowApp()->HandleMsg(winHdl, msg, wParam, lParam);
}

LRESULT WindowApp::HandleMsg(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        running_ = false;
        return 0;
    }

    return DefWindowProc(winHdl, msg, wParam, lParam);
}

WindowApp* WindowApp::InitWindowApp(int width, int height, const char* name) {
    if (windowApp_ != nullptr)
        throw std::runtime_error("Attempt to intialize WindowApp that already exists.");

    return new WindowApp(width, height, name);
}

WindowApp* WindowApp::GetWindowApp() {
    if (windowApp_ == nullptr)
        throw std::runtime_error("Attempt to get WindowApp that doesn't exist.");

    return windowApp_;
}