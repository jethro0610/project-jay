#include "EditorMode.h"
EditorMode::EditorMode(EditorModeArgs args):
camera_(args.camera),
entityManager_(args.entityManager),
inputs_(args.inputs),
levelLoader_(args.levelLoader),
levelProperties_(args.levelProperties),
platform_(args.platform),
renderer_(args.renderer),
resourceManager_(args.resourceManager),
terrain_(args.terrain),
modeText_(args.modeText),
target_(args.target),
textInput_(args.textInput)
{

}

void EditorMode::OnStart() { 
    platform_.SetMouseVisible(mouseVisibile_); 
    modeText_ = GetName();
}
