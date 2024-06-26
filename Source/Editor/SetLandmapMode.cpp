#include "SetLandmapMode.h"
#include "Terrain/Terrain.h"
#include "Level/Level.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"
#include <filesystem>

SetLandmapMode::SetLandmapMode(EditorModeArgs args):
EditorMode(args) {
    mouseVisibile_ = false;
    requiresTarget_ = false;
    requiresLevel_ = true;
    ctrl_ = false;
    reload_ = false;
}

void SetLandmapMode::OnStart() {
    textInput_.SetLabel("lm_");
    EditorMode::OnStart();
}

ConfirmBehavior SetLandmapMode::OnConfirm() {
    std::string landMapName = "lm_" + textInput_.Get();
    if (std::filesystem::exists("landmaps/" + landMapName + ".lmp")) {
        reload_ = false;
        notificaiton_.Set("Loading landmap " + landMapName + "...");
        return CB_PostConfirm;
    }
    else if (std::filesystem::exists("../Assets/landmaps/" + landMapName + ".xcf")) {
        reload_ = true;
        notificaiton_.Set("Loading landmap " + landMapName + "...");
        return CB_PostConfirm;
    }
    else {
        notificaiton_.Set(landMapName + " does not exist");
        return CB_Stay;
    }
    return CB_PostConfirm;
}

ConfirmBehavior SetLandmapMode::PostConfirm() {
    terrain_.DBG_landMapName_ = "lm_" + textInput_.Get();
    if (reload_)
        terrain_.ReloadTerrainDistances(&entities_);
    else
        terrain_.GenerateTerrainDistances(&entities_);
    notificaiton_.Set("Done loading landmap lm_" + textInput_.Get());
    return CB_Default;
}

void SetLandmapMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void SetLandmapMode::Update() {
    textInput_.ReadInput();
}
