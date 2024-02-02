#include "EditorMode.h"
#include "Platform/PC_Platform.h"
#include "Text/Text.h"
#include "EditorTarget.h"
#include "Level/Level.h"

EditorMode::EditorMode(EditorModeArgs args):
camera_(args.camera),
entities_(args.entities),
inputs_(args.inputs),
platform_(args.platform),
renderer_(args.renderer),
resourceManager_(args.resourceManager),
terrain_(args.terrain),
modeText_(args.modeText),
level_(args.level),
notificaiton_(args.notificaiton),
target_(args.target),
textInput_(args.textInput),
visibility_(args.visibility)
{
    requiresEntity_ = false;
    requiresLevel_ = true;
    ctrl_ = false;
}

void EditorMode::OnStart() { 
    platform_.SetMouseVisible(mouseVisibile_); 
    modeText_ = GetName();
}

bool EditorMode::CanSwitch(bool holdingCtrl) const {
    if (requiresTarget_ && !target_.HasTarget())
        return false;

    if (requiresEntity_ && !target_.IsEntity())
        return false;

    if (requiresLevel_ && !level_.loaded_)
        return false;

    if (holdingCtrl != ctrl_)
        return false;

    return true;
}
