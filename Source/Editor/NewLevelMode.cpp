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
    level_.Clear(true);
    level_.loaded_ = true;
    level_.DBG_name_ = "lv_" + textInput_.Get();
    for (int i = 0; i < Level::MAX_PHASES; i++)
        level_.phases_[0] = std::vector<nlohmann::json>();
    level_.DBG_blob_ = "bl_default";

    terrain_.GenerateTerrainHeights(false);
    terrain_.GenerateTerrainDistances("bl_default");
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
