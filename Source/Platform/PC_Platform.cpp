#ifdef __linux__ 
    #define GLFW_EXPOSE_NATIVE_WAYLAND
    #define GETHANDLE(window) (void*)(uintptr_t)glfwGetX11Window(window)
    #define GETDISPLAY() (void*)glfwGetX11Display();
#elif _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GETHANDLE(window) glfwGetWin32Window(window)
    #define GETDISPLAY() nullptr
#endif

#include "PC_Platform.h"
#include "Logging/Logger.h"
#include "Helpers/Assert.h"
#include <assert.h>
#include <fstream>
#include <string>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <bgfx/bgfx.h>

#ifdef __linux
#include <wayland-egl.h>
#endif

Platform* Platform::platform_ = nullptr;

void KeyCallback(GLFWwindow* window, int key, int scancode, int aciton, int mods);
void CursorCallback(GLFWwindow* window, double x, double y);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void LoadMappingFile();

Platform::Platform() {
    assert(platform_ == nullptr);
    DEBUGLOG("Starting PC platform w/ GLFW...");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow(1920, 1080, "Project Jay", NULL, NULL);

    glfwSetKeyCallback(window_, KeyCallback); 

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwGetCursorPos(window_, &mouseX_, &mouseY_);
    glfwSetCursorPosCallback(window_, CursorCallback);
    glfwSetMouseButtonCallback(window_, MouseButtonCallback);
    glfwSetScrollCallback(window_, ScrollCallback);

    LoadMappingFile();

    DEBUGLOG("Starting BGFX...");
    bgfx::Init init; 
    init.type = bgfx::RendererType::Count;
    init.resolution.width = 1920;
    init.resolution.height = 1080;
    init.resolution.reset = BGFX_RESET_NONE;
    init.limits.transientVbSize = 8192 * 8192;

#ifdef __linux__
    wl_surface* surface = glfwGetWaylandWindow(window_);
    wl_egl_window* egl_window = wl_egl_window_create(surface, 1920, 1080);
    glfwSetWindowUserPointer(window_, egl_window);
    init.platformData.nwh = egl_window;
    init.platformData.ndt = glfwGetWaylandDisplay();
    init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
#elif _WIN32
    init.platformData.nwh = GETHANDLE(window_);
    init.platformData.ndt = GETDISPLAY();
#endif

    bgfx::init(init);
    DEBUGLOG("Succesfully started BGFX");
    DEBUGLOG("Succesfully started platform");
    platform_ = this;
}

Platform* Platform::Get() {
    assert(platform_ != nullptr);
    return platform_;
}

GLFWwindow* Platform::GetWindow() {
    assert(platform_ != nullptr);
    return platform_->window_;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0)
        return;

    switch (action) {
        case GLFW_PRESS:
            Platform::Get()->OnKeyDown(key);
            break;

        case GLFW_RELEASE:
            Platform::Get()->OnKeyUp(key);
            break;
    }
}

void Platform::OnKeyDown(int key) {
    heldKeys_[key] = true;
    pressedKeys_[key] = true;
}

void Platform::OnKeyUp(int key) {
    heldKeys_[key] = false;
    releasedKeys_[key] = true;
}

void Platform::FlushKeys() {
    pressedKeys_.reset();
    releasedKeys_.reset();
    heldKeys_[SCROLL_UP_KEY] = false;
    heldKeys_[SCROLL_DOWN_KEY] = false;
}

void CursorCallback(GLFWwindow* window, double x, double y) {
    Platform* platform = Platform::Get();
    platform->mouseX_ = x;
    platform->mouseY_ = y;

    if (platform->changedVisibility_) {
        platform->lastMouseX_ = x;
        platform->lastMouseY_ = y;
        platform->changedVisibility_ = false;
    }
}

void Platform::UpdateMouseMovement() {
    deltaMouseX_ = mouseX_ - lastMouseX_;   
    deltaMouseY_ = mouseY_ - lastMouseY_;
}

void Platform::FlushMouse() {
    deltaMouseX_ = 0;
    deltaMouseY_ = 0;

    lastMouseX_ = mouseX_;
    lastMouseY_ = mouseY_;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button > 1)
        return;

    switch (action) {
        case GLFW_PRESS:
            Platform::Get()->OnKeyDown(MOUSE_KEYS[button]);
            break;

        case GLFW_RELEASE:
            Platform::Get()->OnKeyUp(MOUSE_KEYS[button]);
            break;
    }
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    if (yOffset > 0)
        Platform::Get()->OnKeyDown(SCROLL_UP_KEY);
    else if (yOffset < 0)
        Platform::Get()->OnKeyDown(SCROLL_DOWN_KEY);
}

void Platform::PollGamepad() {
    gamepad_.leftStickX_ = 0.0f;
    gamepad_.leftStickY_ = 0.0f;
    gamepad_.rightStickX_ = 0.0f;
    gamepad_.rightStickY_ = 0.0f;
    gamepad_.leftTrigger_ = 0.0f;
    gamepad_.rightTrigger_ = 0.0f;

    if (!glfwJoystickPresent(GLFW_JOYSTICK_1))
        return;
    
    GLFWgamepadstate state;
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state) == GLFW_FALSE)
        return;

    gamepad_.leftStickX_ = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
    gamepad_.leftStickY_ = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
    gamepad_.rightStickX_ = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
    gamepad_.rightStickY_ = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

    if (gamepad_.leftStickX_ < 0.1f && gamepad_.leftStickX_ > -0.1f)
        gamepad_.leftStickX_ = 0.0f;
    if (gamepad_.leftStickY_ < 0.1f && gamepad_.leftStickY_ > -0.1f)
        gamepad_.leftStickY_ = 0.0f;
    if (gamepad_.rightStickX_ < 0.1f && gamepad_.rightStickX_ > -0.1f)
        gamepad_.rightStickX_ = 0.0f;
    if (gamepad_.rightStickY_ < 0.1f && gamepad_.rightStickY_ > -0.1f)
        gamepad_.rightStickY_ = 0.0f;

    gamepad_.leftTrigger_ = (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) / 2.0f;
    gamepad_.rightTrigger_ = (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;
    gamepad_.SetButtonHeld(Gamepad::LTRIGGER, gamepad_.leftTrigger_ > Gamepad::TRIGGER_LIMIT);
    gamepad_.SetButtonHeld(Gamepad::RTRIGGER, gamepad_.rightTrigger_ > Gamepad::TRIGGER_LIMIT);

    for (int i = 0; i < Gamepad::BUTTONS - 2; i++)
        gamepad_.SetButtonHeld(i, state.buttons[i]);
}

void LoadMappingFile() {
    std::ifstream mappingFile("./mappings/gamecontrollerdb.txt");
    ASSERT(mappingFile.is_open(), "Failed to find input mapping file");

    std::string line, text;
    while (std::getline(mappingFile, line))
        text += line + '\n';

    const char* mappingData = text.c_str();
    ASSERT((glfwUpdateGamepadMappings(mappingData) == GLFW_TRUE), "Input mapping file invalid");
}

void Platform::SetMouseVisible(bool visible) {
    glfwSetInputMode(window_, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    changedVisibility_ = true;
}
