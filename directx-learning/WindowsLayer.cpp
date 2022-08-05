#include "WindowsLayer.h"
#include <stdexcept>
#include <assert.h>  

WindowsLayer* WindowsLayer::windowsLayer_ = nullptr;

WindowsLayer::WindowsLayer(int width, int height, const char* name) {
    windowsLayer_ = this;
    closed_ = false;

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

LRESULT CALLBACK WindowsLayer::HandleMsgThunk(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam) {
    return GetWindowsLayer()->HandleMsg(winHdl, msg, wParam, lParam);
}

LRESULT WindowsLayer::HandleMsg(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        closed_ = true;
        return 0;
    case WM_KEYDOWN:
        OnKeyDown(wParam);
        break;
    case WM_KEYUP:
        OnKeyUp(wParam);
        break;
    }
    return DefWindowProc(winHdl, msg, wParam, lParam);
}

void WindowsLayer::OnKeyDown(unsigned char Key) {
    heldKeys[Key] = true;
    pressedKeys[Key] = true;
}

void WindowsLayer::OnKeyUp(unsigned char Key) {
    heldKeys[Key] = false;
    releasedKeys[Key] = true;
}

void WindowsLayer::ClearPressedAndReleasedKeys() {
    pressedKeys.reset();
    releasedKeys.reset();
}

WindowsLayer* WindowsLayer::InitWindowsLayer(int width, int height, const char* name) {
    assert(windowsLayer_ == nullptr);
    return new WindowsLayer(width, height, name);
}

WindowsLayer* WindowsLayer::GetWindowsLayer() {
    assert(windowsLayer_ != nullptr);
    return windowsLayer_;
}