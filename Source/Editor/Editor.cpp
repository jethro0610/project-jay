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
    modeText_.properties_.hAlignment = Alignment::RIGHT;
    modeText_.properties_.vAlignment = Alignment::BOTTOM;
    modeText_.properties_.hAnchor = Alignment::RIGHT;
    modeText_.properties_.vAnchor = Alignment::BOTTOM;

    target_.name_.properties_.scale = 40.0f;
    target_.name_.properties_.position.y -= 40.0f;
    target_.name_.properties_.hAlignment = Alignment::RIGHT;
    target_.name_.properties_.vAlignment = Alignment::BOTTOM;
    target_.name_.properties_.hAnchor = Alignment::RIGHT;
    target_.name_.properties_.vAnchor = Alignment::BOTTOM;
    target_.name_.properties_.vAnchor = Alignment::BOTTOM;

    textInput_.text_.properties_.scale = 40.0f;
    textInput_.text_.properties_.hAlignment = Alignment::LEFT;
    textInput_.text_.properties_.vAlignment = Alignment::BOTTOM;
    textInput_.text_.properties_.hAnchor = Alignment::LEFT;
    textInput_.text_.properties_.vAnchor = Alignment::BOTTOM;

    notification_.text_.properties_.scale = 40.0f;
    notification_.text_.properties_.position.y -= 40.0f;
    notification_.text_.properties_.hAlignment = Alignment::LEFT;
    notification_.text_.properties_.vAlignment = Alignment::BOTTOM;
    notification_.text_.properties_.hAnchor = Alignment::LEFT;
    notification_.text_.properties_.vAnchor = Alignment::BOTTOM;

    for (int i = 0; i < EntityList::MAX; i++) {
        targets_.push_back(entities_[i].editorTarget_);
    }

    for (StaticTerrainModifier* modifier : terrain.DBG_modifiers_) {
        for (EditorTarget* target : modifier->editorTargets_) {
            targets_.push_back(target);
        }
    }

    for (int i = 0; i < Level::MAX_NAVPOINTS; i++) {
        targets_.push_back(&level.navpoints_[i].target_);
    }

    lastCameraTransform_ = camera_.transform_;
}

void Editor::StartEditing() {
    camera_.transform_ = lastCameraTransform_;
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

void Editor::StopEditing(bool startAtCamera) {
    lastCameraTransform_ = camera_.transform_;

    mode_->OnCancel();
    mode_->OnEnd();
    active_ = false;
    target_.Deselect();
    camera_.target_ = &entities_[0];
    platform_.SetMouseVisible(false);

    // Save the level before we start playing
    level_.Save(level_.DBG_name_, "_autosave");
    level_.Load(level_.DBG_name_, "_autosave", false, false);

    if (startAtCamera)
        entities_[0].transform_.position = camera_.GetLookPosition();
}

void Editor::SetMode(EditorMode* mode) {
    if (mode_ == mode) return;
    if (mode->requiresTarget_ && !target_.HasTarget()) return;
    if (mode->requiresClone_ && !target_.Get().Clonable()) return;

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
        StopEditing(platform_.heldKeys_[GLFW_KEY_LEFT_SHIFT]);
        FlushInputs();
        return;
    }

    std::vector<char> flagIds = target_.Get().GetFlagIDs();
    for (char id : flagIds) {
        if (holdingCtrl && platform_.pressedKeys_[id]) {
            target_.Get().SetFlag(id);
            if (target_.Get().IsModifier())
                terrain_.GenerateTerrainHeights(false, &entities_);
        }
    }

    if (postConfirmMode_ != nullptr) {
        if (postConfirmMode_->PostConfirm() == CB_Default)
            SetMode(defaultMode_);
        postConfirmMode_ = nullptr; 
    }
    else if (mode_ == &defaultMode_) {
        std::unordered_map<char, std::string> bindings = target_.Get().GetScalarNames();
        for (const auto& [id, name] : bindings) {
            if (platform_.pressedKeys_[id] && !holdingCtrl) {
                genericScalarMode_.SetScalarId(id);
                SetMode(genericScalarMode_);
            }
        }

        if (mode_ != &genericScalarMode_) {
            for (EditorMode* mode : modes_) {
                if (mode == &genericScalarMode_)
                    continue;

                if (platform_.pressedKeys_[mode->GetBinding()] && mode->CanSwitch(holdingCtrl)) {
                    SetMode(mode);
                    break;
                }
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
        bool isTransformMode = mode_ == &translateMode_ || mode_ == &scaleMode_ || mode_ == &rotateMode_ || mode_ == &genericScalarMode_;
        if (target_.Get().UpdateTerrain() && isTransformMode)
            terrain_.GenerateTerrainHeights(true, &entities_);
        else if (terrain_.DBG_lowRes_)
            terrain_.GenerateTerrainHeights(false, &entities_);
        else if (modifiersDestroyed)
            terrain_.GenerateTerrainHeights(false, &entities_);

        for (int i = 0; i < EntityList::MAX; i++) {
            if (entities_[i].alive_) {
                entities_[i].CalculateBasePose();
                entities_[i].renderTransform_ = entities_[i].transform_;
                entities_[i].DoEditorUpdate();
            }
        }
        entities_.DestroyFlaggedEntities();

        renderer_.RenderEdit(
            entities_, 
            *this,
            level_,
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
