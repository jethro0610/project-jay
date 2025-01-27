#include "EditorMode.h"
#include "Platform/PC_Platform.h"
#include "Text/Text.h"
#include "EditorTargetController.h"
#include "Level/Level.h"
#include "Camera/Camera.h"
#include "Rendering/Renderer.h"

EditorMode::EditorMode(EditorModeArgs args):
    camera_(args.camera),
    entities_(args.entities),
    inputs_(args.inputs),
    platform_(args.platform),
    renderer_(args.renderer),
    resourceManager_(args.resourceManager),
    spreadManager_(args.spreadManager),
    terrain_(args.terrain),
    terrainCursor_(args.terrainCursor),
    modeText_(args.modeText),
    level_(args.level),
    notificaiton_(args.notificaiton),
    target_(args.target),
    targets_(args.targets),
    textInput_(args.textInput)
{
    requiresClone_ = false;
    requiresLevel_ = true;
    ctrl_ = false;
}

void EditorMode::OnStart() { 
    platform_.SetMouseVisible(mouseVisibile_); 
    modeText_ = GetName();
}

bool EditorMode::CanSwitch(bool holdingCtrl) const {
    if (requiresTarget_ && !target_.HasTarget())
        return false;

    if (requiresClone_ && !target_.Clonable())
        return false;

    if (requiresLevel_ && !level_.loaded_)
        return false;

    if (holdingCtrl != ctrl_)
        return false;

    return true;
}

glm::vec3 EditorMode::GetMouseDirection() {
    float mouseX = platform_.mouseX_ / (1920 * 0.5f) - 1.0f;
    float mouseY = platform_.mouseY_ / (1080 * 0.5f) - 1.0f;

    glm::mat4 invVP = glm::inverse(renderer_.GetProjectionMatrix() * camera_.GetViewOnlyMatrix());
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}
