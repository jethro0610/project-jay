#include "EditorMode.h"
#include "Platform/PC_Platform.h"
#include "Text/Text.h"
#include "EditorTarget.h"
#include "EditorLevel.h"

EditorMode::EditorMode(EditorModeArgs args):
camera_(args.camera),
entityManager_(args.entityManager),
inputs_(args.inputs),
platform_(args.platform),
renderer_(args.renderer),
resourceManager_(args.resourceManager),
terrain_(args.terrain),
modeText_(args.modeText),
level_(args.level),
target_(args.target),
textInput_(args.textInput)
{
    requiresLevel_ = true;
    ctrl_ = false;
}

void EditorMode::OnStart() { 
    platform_.SetMouseVisible(mouseVisibile_); 
    modeText_ = GetName();
}

bool EditorMode::CanSwitch(bool holdingCtrl) const {
    if (requiresTarget_ && target_.Get() == NULL_ENTITY)
        return false;

    if (requiresLevel_ && !level_.hasLevel_)
        return false;

    if (holdingCtrl != ctrl_)
        return false;

    return true;
}
