#include "DefaultMode.h"
#include "Camera/Camera.h"
#include "Platform/PC_Platform.h"
#include "Collision/Ray.h"
#include "Rendering/Renderer.h"
#include "Level/Level.h"
#include "EditorTarget.h"
#include "Terrain/Terrain.h"
#include "EditorVisibility.h"
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

vec3 DefaultMode::GetMouseRay() {
    float mouseX = platform_.mouseX_ / (1280  * 0.5f) - 1.0f;
    float mouseY = platform_.mouseY_ / (720 * 0.5f) - 1.0f;

    glm::mat4 invVP = glm::inverse(renderer_.GetProjectionMatrix() * camera_.GetViewOnlyMatrix());
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}

void DefaultMode::Update() {
    if (platform_.pressedKeys_['V']) {
        target_.Untarget();
        visibility_ = EV_All;
    }
    if (platform_.pressedKeys_['E']) {
        target_.Untarget();
        visibility_ = EV_EntitiesOnly;
    }

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
}

void DefaultMode::CameraUpdate() {
    camera_.Update(inputs_);
}

void DefaultMode::CursorUpdate() {
    if (platform_.pressedKeys_[LEFT_MOUSE_KEY]) {
        target_.Untarget();
        float maxDist = INFINITY;
        vec3 mouseRay = GetMouseRay();

        if (visibility_ == EV_All || visibility_ == EV_TerrainControlsOnly) {
            for (int i = 0; i < terrain_.DBG_bubbles_.size(); i++) {
                vec3 bubblePos = terrain_.DBG_bubbles_[i].position;
                float dist = distance(camera_.transform_.position, bubblePos);
                if (
                    Ray::RayHitSphere(camera_.transform_.position, mouseRay, bubblePos, 4.0f) &&
                    dist < maxDist
                ) {
                    maxDist = dist;
                    target_.SetBubble(&terrain_.DBG_bubbles_[i]);
                }
            }

            for (int i = 0; i < terrain_.DBG_curves_.size(); i++) {
            for (int j = 0; j < 4; j++) {
                vec3 curvePointPos = terrain_.DBG_curves_[i].points[j];
                float dist = distance(camera_.transform_.position, curvePointPos);
                if (
                    Ray::RayHitSphere(camera_.transform_.position, mouseRay, curvePointPos, 4.0f) &&
                    dist < maxDist
                ) {
                    maxDist = dist;
                    target_.SetCurve(&terrain_.DBG_curves_[i], j);
                }
            }}
        }

        if (visibility_ == EV_All || visibility_ == EV_EntitiesOnly) {
            for (int i = 0; i < 128; i++) {
                if (!entities_[i].alive_ || entities_[i].DBG_persistView_) continue;
                Entity& entity = entities_[i];
                float dist = distance(camera_.transform_.position, entity.transform_.position);
                if (
                    Ray::RayHitCollider(camera_.transform_.position, mouseRay, entity.transform_, entity.DBG_collider_) &&
                    dist < maxDist
                ) {
                    maxDist = dist;
                    target_.SetEntity(&entity);
                }
            }
        }
    }
}
