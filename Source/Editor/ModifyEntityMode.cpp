#include "ModifyEntityMode.h"
#include "Entity/Entity.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"
#include "EditorTarget.h"
#include "Entity/EntityTypes.h"

ModifyEntityMode::ModifyEntityMode(EditorModeArgs args):
target_(args.target),
EditorMode(args) {
    requiresEntity_ = true;
    requiresModifyable_ = true;
    requiresLevel_ = true;
    mouseVisibile_ = false;
    ctrl_ = false;
}

void ModifyEntityMode::OnStart() {
    entity_ = target_.GetEntity();
    modeText_ = "Modify Entity";
    switch(entity_->typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)entity_)->DBG_OnStartModify(textInput_, notificaiton_); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    EditorMode::OnStart();
}

void ModifyEntityMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void ModifyEntityMode::OnCancel() {
    switch(entity_->typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)entity_)->DBG_OnCancelModify(textInput_, notificaiton_); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    EditorMode::OnCancel();
}

void ModifyEntityMode::Update() {
    textInput_.ReadInput();
    switch(entity_->typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)entity_)->DBG_UpdateModify(textInput_, notificaiton_); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    EditorMode::Update();
}

ConfirmBehavior ModifyEntityMode::OnConfirm() {
    textInput_.ReadInput();
    switch(entity_->typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: return ((TYPE*)entity_)->DBG_OnConfirmModify(textInput_, notificaiton_); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }

    return CB_Default;
}
