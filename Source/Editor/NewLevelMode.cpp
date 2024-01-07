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

    // DependencyList deps = DependencyList::GenerateFromLevelProperties(
    //     "m_seed_test",
    //     "st_flower_test",
    //     { "m_flower_test", "m_stem_test" },
    //     "m_terrain_grass"
    // );
    // resourceManager_.UnloadUnusedDependencies(deps);
    // resourceManager_.LoadDependencies(deps);
    // level_.properties_.seedMaterial = resourceManager_.GetMaterial("m_seed_test");
    // level_.properties_.spreadModel = resourceManager_.GetModel("st_flower_test");
    // level_.properties_.spreadMaterials.push_back(resourceManager_.GetMaterial("m_flower_test"));
    // level_.properties_.spreadMaterials.push_back(resourceManager_.GetMaterial("m_stem_test"));
    // level_.properties_.terrainMaterial = resourceManager_.GetMaterial("m_terrain_grass");

    // TODO: Assign other level properties
    for (NoiseLayer& noiseLayer : level_.properties_.noiseLayers)
        noiseLayer = {};
    level_.properties_.blob = {};

    // level_.properties_.seedMaterial = resourceManager_.GetMaterial("m_seed_test");

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
