#include "GrowthMode.h"
#include "Spread/SpreadManager.h"
#include "Terrain/Terrain.h"
#include "TerrainCursor.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include "Text/Text.h"
#include <GLFW/glfw3.h>
using namespace glm;

GrowthMode::GrowthMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = true;
    requiresTarget_ = false;
    ctrl_ = false;
    type_ = SpreadType_Flower;
}

std::string GrowthMode::GetName() { 
    switch (type_) {
        case SpreadType_Flower:
            return "Flower Growth";

        case SpreadType_Weed:
            return "Weed Growth";

        default:
            return "Growth";
    }
}

void GrowthMode::OnStart() {
    EditorMode::OnStart();
}

void GrowthMode::OnEnd() {
    terrainCursor_.visible = false;
    EditorMode::OnEnd();
}

void GrowthMode::SetType(SpreadType type) {
    type_ = type;
    modeText_ = GetName();
}

void GrowthMode::Update() {
    if (platform_.pressedKeys_['F'])
        SetType(SpreadType_Flower);

    if (platform_.pressedKeys_['W'])
        SetType(SpreadType_Weed);

    if (platform_.pressedKeys_[GLFW_KEY_UP] || platform_.pressedKeys_[SCROLL_UP_KEY])
        terrainCursor_.radius += 2.0f;

    if (platform_.pressedKeys_[GLFW_KEY_DOWN] || platform_.pressedKeys_[SCROLL_DOWN_KEY])
        terrainCursor_.radius -= 2.0f;

    terrainCursor_.radius = std::max(1.0f, terrainCursor_.radius);

    terrainCursor_.position = terrain_.RaycastTerrain(camera_.transform_.position, GetMouseDirection());
    if (terrainCursor_.position == vec3(0.0f)) {
        terrainCursor_.visible = false;
        return;
    }
    else
        terrainCursor_.visible = true;

    int spreadRadius = terrainCursor_.radius / SpreadManager::SPREAD_DIST;
    if (platform_.heldKeys_[LEFT_MOUSE_KEY] || platform_.heldKeys_[GLFW_KEY_Q])
        spreadManager_.AddSpread(terrainCursor_.position, spreadRadius, 1.0f, SpreadManager::AD_Constant, type_);

    if (platform_.heldKeys_[RIGHT_MOUSE_KEY] || platform_.heldKeys_[GLFW_KEY_E])
        spreadManager_.RemoveSpread(terrainCursor_.position, spreadRadius);
}
