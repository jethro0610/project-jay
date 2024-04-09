#include "Game.h"
#include "Time/Time.h"
#include <GLFW/glfw3.h>

#ifdef _DEBUG
#include "../Logging/ScreenText.h"
#endif

void KeyCallback(GLFWwindow* window, int key, int scancode, int aciton, int mods);

Game::Game():
    timeAccumlulator_(0.0f),
    resourceManager_(),
    particleManager_(),
    terrain_(level_.properties_, resourceManager_),
    level_(
        entities_,
        particleManager_, 
        resourceManager_, 
        seedManager_,
        spreadManager_,
        terrain_
    ),
    renderer_(resourceManager_),
    camera_(18.0f),
    seedManager_(),
    spreadManager_(seedManager_, terrain_),
    entities_(
        rawEntities_, 
        particleManager_, 
        resourceManager_, 
        camera_, 
        inputs_, 
        level_,
        seedManager_, 
        spreadManager_,
        terrain_
    ),
    editor_(
        camera_, 
        entities_, 
        inputs_, 
        level_, 
        particleManager_,
        platform_, 
        resourceManager_,
        renderer_, 
        terrain_,
        running_
    ),
    running_(true)
{
}

void Game::UpdateInputs_P() {
    inputs_.forwardInput = 0.0f;
    inputs_.sideInput = 0.0f;

    inputs_.cut = platform_.heldKeys_[' '] || platform_.gamepad_.heldButtons_[Gamepad::Y];
    inputs_.ski = platform_.heldKeys_[RIGHT_MOUSE_KEY] || platform_.gamepad_.heldButtons_[Gamepad::RTRIGGER];
    inputs_.flow = platform_.heldKeys_[LEFT_MOUSE_KEY] || platform_.gamepad_.heldButtons_[Gamepad::LTRIGGER]; 

    inputs_.startAttack = 
        platform_.pressedKeys_['Q'] || 
        platform_.gamepad_.pressedButtons_[Gamepad::Y] || 
        platform_.gamepad_.pressedButtons_[Gamepad::RSHOULDER];

    inputs_.releaseAttack = 
        platform_.releasedKeys_['Q'] || 
        platform_.gamepad_.releasedButtons_[Gamepad::Y] || 
        platform_.gamepad_.releasedButtons_[Gamepad::RSHOULDER];

    inputs_.startBoost = 
        platform_.pressedKeys_['E'] || 
        platform_.gamepad_.pressedButtons_[Gamepad::A];

    inputs_.releaseBoost = 
        platform_.releasedKeys_['E'] || 
        platform_.gamepad_.releasedButtons_[Gamepad::A];

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
    inputs_.deltaLookX -= platform_.gamepad_.rightStickX_ * GlobalTime::GetDeltaTime() * 2.0f;
    inputs_.deltaLookY += platform_.gamepad_.rightStickY_ * GlobalTime::GetDeltaTime() * 2.0f;
}

void Game::FlushInputs_P() {
    // Have to toggle on flush since released keys are cleared per game update
    // and not per program update. Clearing happens in this function
    #ifdef _DEBUG
    if (platform_.pressedKeys_['`'])
        ScreenText::Toggle();
    
    if (platform_.pressedKeys_[GLFW_KEY_ESCAPE])
        running_ = false;

    if (platform_.heldKeys_[GLFW_KEY_LEFT_CONTROL] && platform_.pressedKeys_['E'])
        editor_.StartEditing();
    #endif

    platform_.FlushKeys();
    platform_.gamepad_.pressedButtons_.reset();
    platform_.gamepad_.releasedButtons_.reset();
}
