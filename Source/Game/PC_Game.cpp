#include "Game.h"
#include "Time.h"

#include <sstream>
#include <iostream>

#ifdef _DEBUG
#include "../Logging/ScreenText.h"
#endif

void KeyCallback(GLFWwindow* window, int key, int scancode, int aciton, int mods);

Game::Game():
    timeAccumlulator_(0.0f),
    resourceManager_(),
    entityManager_(resourceManager_),
    renderer_(platform_.window_),
    camera_(entityManager_.GetComponent<TransformComponent>(), 14.0f),
    world_(resourceManager_),
    seedManager_(),
    spreadManager_(resourceManager_, seedManager_, world_),
    playerController_(),
    running_(true)
{
    Init();

    while (running_) {
        Time::UpdateTime();

        platform_.FlushMouse();
        glfwPollEvents();
        platform_.UpdateMouseMovement();
        platform_.PollGamepad();
        UpdateInputs_P();
        Update();
    }
}

void Game::UpdateInputs_P() {
    inputs_.forwardInput = 0.0f;
    inputs_.sideInput = 0.0f;

    inputs_.cut = platform_.heldKeys_[' '] || platform_.gamepad_.heldButtons_[GAMEPAD_Y];
    inputs_.ski = platform_.heldKeys_[RIGHT_MOUSE_KEY] || platform_.gamepad_.heldButtons_[GAMEPAD_RTRIGGER] || platform_.gamepad_.heldButtons_[GAMEPAD_RSHOULDER];
    inputs_.flow = platform_.heldKeys_[LEFT_MOUSE_KEY] || platform_.gamepad_.heldButtons_[GAMEPAD_LTRIGGER] || platform_.gamepad_.heldButtons_[GAMEPAD_LSHOULDER]; 

    // TODO: clamp the values to total size 1
    if (platform_.heldKeys_['W'])
        inputs_.forwardInput += 1.0f;

    if (platform_.heldKeys_['S'])
        inputs_.forwardInput -= 1.0f;

    if (platform_.heldKeys_['D'])
        inputs_.sideInput += 1.0f;

    if (platform_.heldKeys_['A'])
        inputs_.sideInput -= 1.0f;

    inputs_.forwardInput += platform_.gamepad_.leftStickY_;
    inputs_.sideInput += platform_.gamepad_.leftStickX_;

    inputs_.deltaLookX = 0.0f;
    inputs_.deltaLookY = 0.0f;
    inputs_.deltaLookX -= platform_.deltaMouseX_ * 0.005f;
    inputs_.deltaLookY -= platform_.deltaMouseY_ * 0.005f;
    inputs_.deltaLookX -= platform_.gamepad_.rightStickX_ * Time::GetDeltaTime() * 2.0f;
    inputs_.deltaLookY += platform_.gamepad_.rightStickY_ * Time::GetDeltaTime() * 2.0f;
}

void Game::FlushInputs_P() {
    // Have to toggle on flush since released keys are cleared per game update
    // and not per program update. Clearing happens in this function
    #ifdef _DEBUG
    if (platform_.pressedKeys_['Q'])
        ScreenText::Toggle();
    
    if (platform_.pressedKeys_[GLFW_KEY_ESCAPE])
        running_ = false;
    #endif

    platform_.FlushKeys();
    platform_.gamepad_.pressedButtons_.reset();
    platform_.gamepad_.releasedButtons_.reset();
}