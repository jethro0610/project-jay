#include "WeedMode.h"
#include "Spread/SpreadManager.h"
#include "Terrain/Terrain.h"
#include "TerrainCursor.h"
#include "Camera/Camera.h"
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

void WeedMode::OnStart() {
    terrainCursor_.visible = true;
    terrainCursor_.radius = 1.0f;
    EditorMode::OnStart();
}

void WeedMode::OnEnd() {
    terrainCursor_.visible = false;
    EditorMode::OnEnd();
}

void WeedMode::Update() {
    terrainCursor_.position = terrain_.RaycastTerrain(camera_.transform_.position, GetMouseRay());
}
