#include "Editor.h"
#include "Camera/Camera.h"
#include "Level/Level.h"
#include "Rendering/Renderer.h"
#include "Terrain/Terrain.h"
#include "Particle/ParticleManager.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
#include <GLFW/glfw3.h>
using namespace glm;

Editor::Editor(
    Camera& camera, 
    EntityList& entities, 
    Inputs& inputs,
    Level& level,
    ParticleManager& particleManager,
    Platform& platform, 
    ResourceManager& resourceManager,
    Renderer& renderer,
    SpreadManager& spreadManager,
    Terrain& terrain,
    bool& running
):
camera_(camera),
entities_(entities),
inputs_(inputs),
level_(level),
particleManager_(particleManager),
platform_(platform),
resourceManager_(resourceManager),
renderer_(renderer),
spreadManager_(spreadManager),
terrain_(terrain),
running_(running),
target_(),
textInput_(platform),
args_({
    camera, 
    entities, 
    inputs, 
    level,
    platform, 
    renderer, 
    resourceManager, 
    spreadManager,
    terrain, 
    terrainCursor_,
    modeText_,
    notification_,
    target_, 
    targets_,
    textInput_
}),
#define MODEEXPANSION(TYPE, VAR) VAR(args_),
#define TAILMODEEXPANSION(TYPE, VAR) VAR(args_)
EXPANDMODES 
#undef MODEEXPANSION 
#undef TAILMODEEXPANSION
{
    mode_ = nullptr;
    #define MODEEXPANSION(TYPE, VAR) modes_.push_back(&VAR);
    #define TAILMODEEXPANSION(TYPE, VAR) modes_.push_back(&VAR);
    EXPANDMODES 
    #undef MODEEXPANSION 
    #undef TAILMODEEXPANSION

    active_ = false;

    modeText_.properties_.scale = 40.0f;
    modeText_.properties_.hAlignment = Text::RIGHT_ALIGN;
    modeText_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    modeText_.properties_.hAnchor = Text::RIGHT_ALIGN;
    modeText_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    target_.name_.properties_.scale = 40.0f;
    target_.name_.properties_.position.y -= 40.0f;
    target_.name_.properties_.hAlignment = Text::RIGHT_ALIGN;
    target_.name_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    target_.name_.properties_.hAnchor = Text::RIGHT_ALIGN;
    target_.name_.properties_.vAnchor = Text::BOTTOM_ALIGN;
    target_.name_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    textInput_.text_.properties_.scale = 40.0f;
    textInput_.text_.properties_.hAlignment = Text::LEFT_ALIGN;
    textInput_.text_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    textInput_.text_.properties_.hAnchor = Text::LEFT_ALIGN;
    textInput_.text_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    notification_.text_.properties_.scale = 40.0f;
    notification_.text_.properties_.position.y -= 40.0f;
    notification_.text_.properties_.hAlignment = Text::LEFT_ALIGN;
    notification_.text_.properties_.vAlignment = Text::BOTTOM_ALIGN;
    notification_.text_.properties_.hAnchor = Text::LEFT_ALIGN;
    notification_.text_.properties_.vAnchor = Text::BOTTOM_ALIGN;

    for (int i = 0; i < 128; i++)
        targets_.push_back(entities_[i].editorTarget_);

    for (StaticTerrainModifier* modifier : terrain.DBG_modifiers_) {
        for (EditorTarget* target : modifier->editorTargets_) {
            targets_.push_back(target);
        }
    }
}

void Editor::StartEditing() {
    active_ = true;
    target_.Deselect();
    SetMode(defaultMode_);
    defaultMode_.SetSubmode(DS_Camera);
    postConfirmMode_ = nullptr;
    camera_.target_ = nullptr;
    ScreenText::SetEnabled(false);
    notification_.Clear();

    // Reload the level once we start editing again
    if (level_.loaded_)
        level_.Load(level_.DBG_name_, "_autosave", false, true);
}

void Editor::StopEditing() {
    mode_->OnCancel();
    mode_->OnEnd();
    active_ = false;
    target_.Deselect();
    camera_.target_ = &entities_[0];
    platform_.SetMouseVisible(false);

    // Save the level before we start playing
    level_.Save(level_.DBG_name_, "_autosave");
    level_.Load(level_.DBG_name_, "_autosave", false, false);
}

void Editor::SetMode(EditorMode* mode) {
    if (mode_ == mode) return;
    if (mode->requiresTarget_ && !target_.HasTarget()) return;
    if (mode->requiresClone_ && !target_.Clonable()) return;

    if (mode_ != nullptr)
        mode_->OnEnd();

    mode_ = mode;
    FlushInputs();
    mode_->OnStart();
}

void Editor::SetMode(EditorMode& mode) {
    SetMode(&mode);
}

void Editor::Update() {
    notification_.Update();

    bool holdingCtrl = platform_.heldKeys_[GLFW_KEY_LEFT_CONTROL];

    bool shouldReloadLandmap = false;
    if (holdingCtrl && platform_.pressedKeys_['L'] && level_.loaded_) {
        notification_.Set("Reloading landmap...");
        shouldReloadLandmap = true;
    }

    if (holdingCtrl && platform_.pressedKeys_['E'] && level_.loaded_) {
        StopEditing();
        FlushInputs();
        return;
    }

    if (postConfirmMode_ != nullptr) {
        if (postConfirmMode_->PostConfirm() == CB_Default)
            SetMode(defaultMode_);
        postConfirmMode_ = nullptr; 
    }
    else if (mode_ == &defaultMode_) {
        for (EditorMode* mode : modes_) {
            if (platform_.pressedKeys_[mode->GetBinding()] && mode->CanSwitch(holdingCtrl)) {
                SetMode(mode);
                break;
            }
        }
    }
    else if (platform_.pressedKeys_[defaultMode_.GetBinding()]) {
        mode_->OnCancel();
        SetMode(defaultMode_);
    }
    else if (platform_.pressedKeys_[GLFW_KEY_ENTER]) {
        switch(mode_->OnConfirm()) {
            case CB_Default:
                SetMode(defaultMode_);
                break;

            case CB_Stay:
                break;

            case CB_PostConfirm:
                postConfirmMode_ = mode_;
                break;
        }
    }

    if (postConfirmMode_ == nullptr)
        mode_->Update();
    
    if (level_.loaded_) {
        bool modifiersDestroyed = terrain_.DestroyPendingModifiers();
        bool isTransformMode = mode_ == &translateMode_ || mode_ == &scaleMode_ || mode_ == &rotateMode_;
        if (target_.UpdateTerrain() && isTransformMode)
            terrain_.GenerateTerrainHeights(true, &entities_);
        else if (terrain_.DBG_lowRes_)
            terrain_.GenerateTerrainHeights(false, &entities_);
        else if (modifiersDestroyed)
            terrain_.GenerateTerrainHeights(false, &entities_);

        for (int i = 0; i < 128; i++) {
            if (entities_[i].alive_) {
                entities_[i].CalculateBasePose();
                entities_[i].renderTransform_ = entities_[i].transform_;
            }
        }
        entities_.DestroyFlaggedEntities();

        renderer_.RenderEdit(
            entities_, 
            *this,
            level_.properties_,
            spreadManager_,
            terrain_,
            terrainCursor_
        );
        particleManager_.Update(0.0f); // Need this to free unused emitters
    }
    else {
        renderer_.RenderEditorOnly(*this);
    }
    FlushInputs();

    if (shouldReloadLandmap) {
        terrain_.ReloadTerrainDistances(&entities_);
        notification_.Set("Done reloading landmap");
    }
}

// Since editor is PC only this will just be in the regular file
void Editor::FlushInputs() {
    // Have to toggle on flush since released keys are cleared per game update
    // and not per program update. Clearing happens in this function
    if (platform_.pressedKeys_[GLFW_KEY_ESCAPE])
        running_ = false;

    platform_.FlushKeys();
    platform_.gamepad_.pressedButtons_.reset();
    platform_.gamepad_.releasedButtons_.reset();
}
