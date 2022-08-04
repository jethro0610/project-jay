#pragma once
#include <Windows.h>

class WindowApp {
private:
    static WindowApp* windowApp_;
    WindowApp(int width, int height, const char* name);

public:
    const char* className_ = "WindowAppClass";
    HINSTANCE classInstance_;

    HWND windowHandle_;
    bool running_;

    static LRESULT CALLBACK HandleMsgThunk(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND winHdl, UINT msg, WPARAM wParam, LPARAM lParam);

    static WindowApp* InitWindowApp(int width, int height, const char* name);
    static WindowApp* GetWindowApp();
};

