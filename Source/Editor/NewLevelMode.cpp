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
    std::string levelName = "lv_" + textInput_.Get();

    if (std::filesystem::exists("levels/" + levelName + ".json")) {
        notificaiton_.Set(levelName + " already exists");
        return CB_Stay;
    }
    else {
        notificaiton_.Set("Creating " + levelName + "...");
        return CB_PostConfirm;
    }
}

ConfirmBehavior NewLevelMode::PostConfirm() {
    level_.Clear(true);
    level_.loaded_ = true;
    level_.DBG_name_ = "lv_" + textInput_.Get();
    for (int i = 0; i < Level::MAX_PHASES; i++)
        level_.phases_[0] = std::vector<nlohmann::json>();

    terrain_.DBG_landMapName_ = "lm_default";
    terrain_.GenerateTerrainDistances();
    terrain_.GenerateTerrainHeights(false);

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
