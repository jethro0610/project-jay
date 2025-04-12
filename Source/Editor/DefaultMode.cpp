#include "DefaultMode.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include "Collision/Ray.h"
#include "Rendering/Renderer.h"
#include "EditorTarget.h"
#include "EditorTargetController.h"
#include "Entity/Entity.h"
using namespace glm;

DefaultMode::DefaultMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = false;
    ctrl_ = false;
    submode_ = DS_Camera;
}

void DefaultMode::OnStart() {
    submode_ = DS_Camera;
    EditorMode::OnStart();
}

std::string DefaultMode::GetName() { 
    switch (submode_)  {
        case DS_Camera:
            return "Camera";

        case DS_Cursor:
            return "Cursor";
        
        default:
            return "Default";
    }
}

void DefaultMode::SetSubmode(DefaultSubmode submode) {
    submode_ = submode;
    modeText_ = GetName();
    switch (submode_)  {
        case DS_Camera:
            platform_.SetMouseVisible(false);
            break;

        case DS_Cursor:
            platform_.SetMouseVisible(true);
            break;
        
        default:
            break;
    }
}

void DefaultMode::Update() {
    if (platform_.pressedKeys_[GetBinding()]) {
        switch(submode_) {
            case DS_Camera:
                SetSubmode(DS_Cursor);
                break;

            case DS_Cursor:
                SetSubmode(DS_Camera);
                break;

            default:
                break;
        }
    }

    switch(submode_) {
        case DS_Camera:
            CameraUpdate();
            break;

        case DS_Cursor:
            CursorUpdate();
            break;

        default:
            break;
    }

    if (platform_.pressedKeys_[GLFW_KEY_DELETE] && target_.HasTarget()) {
        target_.Destroy();
    }

    if (platform_.pressedKeys_['P'] && target_.IsEntity()) {
        target_.TogglePreview();
    }
}

void DefaultMode::CameraUpdate() {
    camera_.Update(inputs_);
}

void DefaultMode::CursorUpdate() {
    if (platform_.pressedKeys_[LEFT_MOUSE_KEY]) {
        target_.Deselect();
        float maxDist = INFINITY;
        Ray ray(camera_.transform_.position, GetMouseDirection());

        for (EditorTarget* target : targets_) {
            if (target->Selectable() && target->RayHit(ray)) {
                target_.Select(target);
                break;
            }
        }
    }
}
