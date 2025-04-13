#include "TranslateMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityList.h"
#include "Platform/PC_Platform.h"
#include "Terrain/Terrain.h"
#include "EditorTargetController.h"
using namespace glm;

TranslateMode::TranslateMode(EditorModeArgs args):
EditorMode(args) 
{
    mouseVisibile_ = false;
    requiresTarget_ = true;
}

std::string TranslateMode::GetName() { 
    switch (submode_) {
        case TS_Planar:
            return "Planar Translate";

        case TS_Terrain:
            return "Terrain Translate";

        case TS_Vertical:
            return "Vertical Translate";

        default:
            return "Translate";
    }
}

void TranslateMode::OnStart() {
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;
    submode_ = TS_Planar;
    startPosition_ = target_.Get().GetPosition();

    EditorMode::OnStart();
}

void TranslateMode::OnCancel() {
    target_.Get().SetPosition(startPosition_);
}

void TranslateMode::SetSubmode(TranslateSubmode submode) {
    if (submode_ == submode) return;

    if (submode == TS_Vertical || submode_ == TS_Vertical) {
        deltaX_ = 0.0f;
        deltaY_ = 0.0f;
    }

    submode_ = submode;
    modeText_ = GetName();
}

void TranslateMode::Update() {
    deltaX_ += platform_.deltaMouseX_ * 0.1f;
    deltaY_ -= platform_.deltaMouseY_ * 0.1f;

    if (platform_.pressedKeys_['P'])
        SetSubmode(TS_Planar);
    else if (platform_.pressedKeys_['V'])
        SetSubmode(TS_Vertical);
    if (platform_.pressedKeys_['T'] && target_.Get().TerrainAlignable())
        SetSubmode(TS_Terrain);

    vec3 planarCameraForward = camera_.transform_.GetForwardVector();
    planarCameraForward.y = 0.0f;
    planarCameraForward = normalize(planarCameraForward);

    vec3 planarCameraRight = camera_.transform_.GetRightVector();
    planarCameraRight.y = 0.0f;
    planarCameraRight = normalize(planarCameraRight);

    switch (submode_) {
        case TS_Planar:
            target_.Get().SetPosition(
                startPosition_ +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_
            );
            break;

        case TS_Vertical:
            target_.Get().SetPosition(startPosition_ + Transform::worldUp * deltaY_);
            break;

        case TS_Terrain:
            vec3 terrainPos =
                startPosition_ +
                planarCameraForward * deltaY_ +
                planarCameraRight * deltaX_;
            float height = terrain_.GetHeight(vec2(terrainPos.x, terrainPos.z));
            if (height > -INFINITY)
                terrainPos.y = terrain_.GetHeight(vec2(terrainPos.x, terrainPos.z));
            target_.Get().SetPosition(terrainPos);
            break;
    }
}
