#include "WindowsLayer.h"
#include <assert.h>  

WindowsLayer* WindowsLayer::windowsLayer_ = nullptr;

WindowsLayer::WindowsLayer(int width, int height, const char* name) {
    assert(windowsLayer_ == nullptr);
    windowsLayer_ = this;
    closed_ = false;

    rawMouse_ = true;
    mouseX_ = 0;
    mouseY_ = 0;
    lastMouseX_ = 0;
    lastMouseY_ = 0;
    deltaMouseX_ = 0;
    deltaMouseY_ = 0;

    // Create and register the Window class
    WNDCLASS windowClass = {};
    windowClass.style;
    windowClass.lpfnWndProc = HandleMsgThunk;
    classInstance_ = GetModuleHandle(nullptr);
    windowClass.hInstance = classInstance_;
    windowClass.lpszClassName = className_;
    windowClass.style = CS_OWNDC;
    RegisterClass(&windowClass);

    DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    // Calculate the window size
    RECT wRect = {0, 0, width, height};
    AdjustWindowRect(&wRect, windowStyle, FALSE);

    // Create the window and its handle
    windowHandle_ = CreateWindow(
        className_,
        name,
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wRect.right - wRect.left,
        wRect.bottom - wRect.top,
        nullptr,
        nullptr,
        classInstance_,
        this
    );
    ShowWindow(windowHandle_, SW_SHOW);

    // Setup raw input
    rawInputDevice_[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    rawInputDevice_[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    rawInputDevice_[0].dwFlags = RIDEV_INPUTSINK;
    rawInputDevice_[0].hwndTarget = windowHandle_;
    RegisterRawInputDevices(rawInputDevice_, 1, sizeof(rawInputDevice_[0]));

    ShowWindow(windowHandle_, SW_SHOWDEFAULT);
}

LRESULT CALLBACK WindowsLayer::HandleMsgThunk(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam) {
    return GetWindowsLayer()->HandleMsg(winHdl, msg, wParam, lParam);
}

LRESULT WindowsLayer::HandleMsg(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE: {
        closed_ = true;
        return 0;
    }
    case WM_QUIT: {
        closed_ = true;
        return 0;
    }

    case WM_KEYDOWN: {
        OnKeyDown(uint16_t(wParam));
    } break;

    case WM_KEYUP: {
        OnKeyUp(uint16_t(wParam));
    } break;

    case WM_LBUTTONDOWN: {
        OnKeyDown(LEFT_MOUSE_KEY);
    } break;

    case WM_LBUTTONUP: {
        OnKeyUp(LEFT_MOUSE_KEY);
    } break;

    case WM_RBUTTONDOWN: {
        OnKeyDown(RIGHT_MOUSE_KEY);
    } break;

    case WM_RBUTTONUP: {
        OnKeyUp(RIGHT_MOUSE_KEY);
    } break;

    case WM_MOUSEMOVE: {
        const POINTS point = MAKEPOINTS(lParam);
        OnMouseMove(point.x, point.y);
    } break;

    case WM_INPUT: {
        if (!rawMouse_)
            break;

        UINT dwSize = sizeof(RAWINPUT);
        static BYTE lpb[sizeof(RAWINPUT)];

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
        RAWINPUT* raw = (RAWINPUT*)lpb;

        if (raw->header.dwType == RIM_TYPEMOUSE) {
            deltaMouseX_ += raw->data.mouse.lLastX;
            deltaMouseY_ += raw->data.mouse.lLastY;
        }
    } break;

    default:
        break;
    }
    return DefWindowProc(winHdl, msg, wParam, lParam);
}

void WindowsLayer::OnKeyDown(uint16_t key) {
    heldKeys_[key] = true;
    pressedKeys_[key] = true;
}

void WindowsLayer::OnKeyUp(uint16_t key) {
    heldKeys_[key] = false;
    releasedKeys_[key] = true;
}

void WindowsLayer::OnMouseMove(int x, int y) {
    mouseX_ = x;
    mouseY_ = y;
}

void WindowsLayer::ClearPressedAndReleasedKeys() {
    pressedKeys_.reset();
    releasedKeys_.reset();
}

void WindowsLayer::ClearMouseMovement() {
    deltaMouseX_ = 0;
    deltaMouseY_ = 0;

    lastMouseX_ = mouseX_;
    lastMouseY_ = mouseY_;
}

void WindowsLayer::UpdateMouseMovement() {
    if (rawMouse_)
        return;

    deltaMouseX_ = mouseX_ - lastMouseX_;
    deltaMouseY_ = mouseY_ - lastMouseY_;
}

WindowsLayer* WindowsLayer::GetWindowsLayer() {
    assert(windowsLayer_ != nullptr);
    return windowsLayer_;
}
