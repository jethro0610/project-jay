#include "NewLevelMode.h"
#include "Resource/ResourceManager.h"
#include "Resource/DependencyList.h"
#include "Terrain/Terrain.h"
#include "Level/Level.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"

NewLevelMode::NewLevelMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = false;
    ctrl_ = true;
}

void NewLevelMode::OnStart() {
    textInput_.SetLabel("lv_");
    EditorMode::OnStart();
}

ConfirmBehavior NewLevelMode::OnConfirm() {
    notificaiton_.Set("Creating lv_" + textInput_.Get() + "...");
    return CB_PostConfirm;
}

ConfirmBehavior NewLevelMode::PostConfirm() {
    level_.Clear();
    level_.loaded_ = true;
    level_.DBG_name_ = "lv_" + textInput_.Get();
    for (int i = 0; i < 4; i++)
        level_.phases_[0] = std::vector<nlohmann::json>();

    // TODO: Assign other level properties
    for (NoiseLayer& noiseLayer : level_.properties_.noiseLayers)
        noiseLayer = {};
    level_.properties_.blob = {};

    terrain_.GenerateTerrainMap(level_.properties_.noiseLayers, level_.properties_.blob);
    notificaiton_.Set("Created lv_" + textInput_.Get());
    return CB_Default;
}

void NewLevelMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void NewLevelMode::Update() {
    textInput_.ReadInput();
}
