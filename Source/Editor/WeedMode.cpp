#include "WeedMode.h"
#include "Spread/SpreadManager.h"
#include "Terrain/Terrain.h"
#include "TerrainCursor.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include <GLFW/glfw3.h>
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
    EditorMode::OnStart();
}

void WeedMode::OnEnd() {
    terrainCursor_.visible = false;
    EditorMode::OnEnd();
}

void WeedMode::Update() {
    if (platform_.pressedKeys_[GLFW_KEY_UP])
        terrainCursor_.radius += 2.0f;

    if (platform_.pressedKeys_[GLFW_KEY_DOWN])
        terrainCursor_.radius -= 2.0f;

    terrainCursor_.radius = std::max(1.0f, terrainCursor_.radius);

    terrainCursor_.position = terrain_.RaycastTerrain(camera_.transform_.position, GetMouseRay());
    if (terrainCursor_.position == vec3(0.0f)) {
        terrainCursor_.visible = false;
        return;
    }
    else
        terrainCursor_.visible = true;

    int spreadRadius = terrainCursor_.radius / SpreadManager::SPREAD_DIST;
    if (platform_.heldKeys_[LEFT_MOUSE_KEY])
        spreadManager_.AddSpread(terrainCursor_.position, spreadRadius, INT_MAX, true);
}
