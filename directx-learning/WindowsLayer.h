#pragma once
#include <Windows.h>
#include <bitset>
#include <queue>
class WindowsLayer {
private:
    static WindowsLayer* windowsLayer_;
    WindowsLayer(int width, int height, const char* name);

public:
    const char* className_ = "WindowAppClass";
    HINSTANCE classInstance_;

    HWND windowHandle_;
    bool closed_;

    std::bitset<256> heldKeys;
    std::bitset<256> pressedKeys;
    std::bitset<256> releasedKeys;

    void ClearPressedAndReleasedKeys();

    static LRESULT CALLBACK HandleMsgThunk(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam);

    static WindowsLayer* InitWindowsLayer(int width, int height, const char* name);
    static WindowsLayer* GetWindowsLayer();

private:
    void OnKeyDown(unsigned char Key);
    void OnKeyUp(unsigned char Key);
};

