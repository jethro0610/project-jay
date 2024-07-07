#include "WeedMode.h"
#include "Spread/SpreadManager.h"
#include "Terrain/Terrain.h"
using namespace glm;

WeedMode::WeedMode(EditorModeArgs args):
EditorMode(args),
spreadManager_(args.spreadManager)
{
    mouseVisibile_ = true;
    requiresTarget_ = false;
    ctrl_ = false;
}

std::string WeedMode::GetName() { 
    return "Create Weeds";
}

void WeedMode::OnStart() {
    EditorMode::OnStart();
}

void WeedMode::Update() {
}
