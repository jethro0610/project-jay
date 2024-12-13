#include "OpenLevelMode.h"
#include "Level/Level.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"
#include <filesystem>

OpenLevelMode::OpenLevelMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = false;
    ctrl_ = true;
}

void OpenLevelMode::OnStart() {
    textInput_.SetLabel("lv_");
    EditorMode::OnStart();
}

ConfirmBehavior OpenLevelMode::OnConfirm() {
    std::string levelName = "lv_" + textInput_.Get();
    if (std::filesystem::exists("levels/" + levelName + ".json")) {
        notificaiton_.Set("Loading " + levelName + "...");
        return CB_PostConfirm;
    }
    else {
        notificaiton_.Set(levelName + " does not exist");
        return CB_Stay;
    }
}

ConfirmBehavior OpenLevelMode::PostConfirm() {
    std::string levelName = "lv_" + textInput_.Get();
    level_.Load(levelName, "", true, true);
    notificaiton_.Set("Loaded " + levelName); 
    return CB_Default;
}

void OpenLevelMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void OpenLevelMode::Update() {
    textInput_.ReadInput();
}
