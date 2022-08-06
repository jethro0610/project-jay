#pragma once
#include <Windows.h>
#include <bitset>
#include <hidusage.h>

#define MOUSE_KEYS 2
#define LEFT_MOUSE_KEY 255 + 1
#define RIGHT_MOUSE_KEY 255 + 2

class WindowsLayer {
private:
    static WindowsLayer* windowsLayer_;
    WindowsLayer(int width, int height, const char* name);

public:
    const char* className_ = "WindowAppClass";
    HINSTANCE classInstance_;

    HWND windowHandle_;
    bool closed_;

    std::bitset<256 + MOUSE_KEYS> heldKeys_;
    std::bitset<256 + MOUSE_KEYS> pressedKeys_;
    std::bitset<256 + MOUSE_KEYS> releasedKeys_;

    RAWINPUTDEVICE rawInputDevice_[1];
    bool rawMouse_;
    int mouseX_;
    int mouseY_;
    int lastMouseX_;
    int lastMouseY_;
    int deltaMouseX_;
    int deltaMouseY_;
    void ClearPressedAndReleasedKeys();
    void ClearMouseMovement();
    void UpdateMouseMovement();

    static LRESULT CALLBACK HandleMsgThunk(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam);

    static WindowsLayer* InitWindowsLayer(int width, int height, const char* name);
    static WindowsLayer* GetWindowsLayer();

private:
    void OnKeyDown(uint16_t key);
    void OnKeyUp(uint16_t key);
    void OnMouseMove(int x, int y);
};

