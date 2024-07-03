#include "WeedMode.h"
using namespace glm;

WeedMode::WeedMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = true;
    requiresTarget_ = false;
    ctrl_ = false;
}

std::string WeedMode::GetName() { 
    return "Create Weeds";
}

void WeedMode::Update() {
}
