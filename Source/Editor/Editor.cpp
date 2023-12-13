#include "Editor.h"
#include "Rendering/Renderer.h"
#include "Systems/Systems.h"
#include <GLFW/glfw3.h>

Editor::Editor(
    Camera& camera, 
    EntityManager& entityManager, 
    Inputs& inputs,
    LevelLoader& levelLoader,
    LevelProperties& levelProperties,
    Platform& platform, 
    Renderer& renderer,
    Terrain& terrain,
    bool& running
):
camera_(camera),
entityManager_(entityManager),
inputs_(inputs),
levelLoader_(levelLoader),
levelProperties_(levelProperties),
platform_(platform),
renderer_(renderer),
terrain_(terrain),
running_(running)
{
    active_ = false;
    target_ = NULL_ENTITY;
    mode_ = EM_Camera;
}

void Editor::StartEditing() {
    active_ = true;
    target_ = NULL_ENTITY;
    mode_ = EM_Camera;
    camera_.target_ = NULL_ENTITY;
    // ScreenText::Clear();
    ScreenText::SetEnabled(false);
    levelLoader_.LoadLevel();
}

void Editor::StopEditing() {
    active_ = false;
    target_ = NULL_ENTITY;
    camera_.target_ = PLAYER_ENTITY;
    ScreenText::Clear();
    ScreenText::SetEnabled(false);
}

void Editor::SetMode(EditorMode mode) {
    mode_ = mode;
    switch (mode_) {
        case EM_Camera: {
            platform_.SetMouseVisible(false); 
            break;
        }

        case EM_Mouse: {
            platform_.SetMouseVisible(true); 
            break;
        }

        default:
            break;
    }
}

void Editor::Update() {
    if (platform_.pressedKeys_['1'])
        SetMode(EM_Camera);
    if (platform_.pressedKeys_['2'])
        SetMode(EM_Mouse);
    if (platform_.pressedKeys_['Q'])
        StopEditing();

    if (mode_ == EM_Camera)
        camera_.Update(inputs_);

    TransformSystem::ForceRenderTransforms(
        entityManager_.entities_,
        entityManager_.components_
    );
    renderer_.RenderMinimal(
        entityManager_.entities_, 
        entityManager_.components_,
        levelProperties_,
        terrain_ 
    );
    FlushInputs();
}

// Since editor is PC only this will just be in the regular file
void Editor::FlushInputs() {
    // Have to toggle on flush since released keys are cleared per game update
    // and not per program update. Clearing happens in this function
    #ifdef _DEBUG
    if (platform_.pressedKeys_[GLFW_KEY_ESCAPE])
        running_ = false;
    #endif

    platform_.FlushKeys();
    platform_.gamepad_.pressedButtons_.reset();
    platform_.gamepad_.releasedButtons_.reset();
}
