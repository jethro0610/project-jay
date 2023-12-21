#include "TerrainEditMode.h"
#include "Entity/EntityManager.h"
#include "Level/LevelProperties.h"
#include "Terrain/Terrain.h"
#include "EditorTextInput.h"
#include "Helpers/StringConversion.h"
#include <GLFW/glfw3.h>
#include <format>

TerrainEditMode::TerrainEditMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
}

void TerrainEditMode::OnStart() {
    EditorMode::OnStart();
    SetPhase(TE_SelectNoiseLayer);
}

void TerrainEditMode::OnEnd() {
    textInput_.Clear();
}

void TerrainEditMode::SetPhase(TerrainEditPhase phase) {
    phase_ = phase;
    switch (phase_) {
        case TE_SelectNoiseLayer: {
            textInput_.SetLabel("Layer (1-4): ");
            break;
        }

        case TE_SelectProperty: {
            textInput_.SetLabel("Modify Layer " + LayerName() + " (d/s/f/m/e): ");
            break;
        }

        case TE_Activate: {
            textInput_.SetLabel("Activate Layer " + LayerName() + " (y/n): ");
            break;
        }

        case TE_Deactivate: {
            textInput_.SetLabel("Deactivate Layer " + LayerName() + " (y/n): ");
            break;
        }

        case TE_Seed: { 
            std::string seed = std::to_string(levelProperties_.noiseLayers[targetLayer_].seed);
            textInput_.SetLabel("Set Layer " + LayerName() + " Seed (" + seed + "): "); 
            break;
        }

        case TE_Frequency: {
            std::string xFreq = std::format("{:.3f}", levelProperties_.noiseLayers[targetLayer_].frequency.x);
            std::string yFreq = std::format("{:.3f}", levelProperties_.noiseLayers[targetLayer_].frequency.y);
            textInput_.SetLabel("Set Layer " + LayerName() + " Frequency (" + xFreq + "," + yFreq + "): "); 
            break;
        }

        case TE_Multiplier: {
            std::string mult = std::format("{:.3f}", levelProperties_.noiseLayers[targetLayer_].multiplier);
            textInput_.SetLabel("Set Layer " + LayerName() + " Multiplier (" + mult + "): "); 
            break;
        }

        case TE_Exponent: {
            std::string exp = std::format("{:.3f}", levelProperties_.noiseLayers[targetLayer_].exponent);
            textInput_.SetLabel("Set Layer " + LayerName() + " Exponent (" + exp + "): "); 
            break;
        }
    }
}

void TerrainEditMode::Update() {
    textInput_.ReadInput();
}

bool TerrainEditMode::OnConfirm() {
    const std::string& input = textInput_.Get();
    bool modified = false;
    bool exit = false;
    switch(phase_) {
        case TE_SelectNoiseLayer: {
            StringToInt layer = ToInt(input);
            if (layer.valid && layer.value >= 1 && layer.value <= 4) {
                targetLayer_ = layer.value - 1;
                if (levelProperties_.noiseLayers[targetLayer_].active)
                    SetPhase(TE_SelectProperty);
                else
                    SetPhase(TE_Activate);
            }
            else if (input == "b") {
                exit = true;
            }
            break;
        }

        case TE_SelectProperty: {
            if (input == "d")
                SetPhase(TE_Deactivate);
            else if (input == "s")
                SetPhase(TE_Seed);
            else if (input == "f")
                SetPhase(TE_Frequency);
            else if (input == "m")
                SetPhase(TE_Multiplier);
            else if (input == "e")
                SetPhase(TE_Exponent);
            else if (input == "b")
                SetPhase(TE_SelectNoiseLayer);
            break;
        }

        case TE_Activate: {
            if (input == "y") {
                levelProperties_.noiseLayers[targetLayer_].active = true;
                modified = true;
                SetPhase(TE_SelectProperty);
            }
            else if (input == "n" || input == "b") {
                SetPhase(TE_SelectNoiseLayer);
            }
            break;
        }

        case TE_Deactivate: {
            if (input == "y") {
                levelProperties_.noiseLayers[targetLayer_].active = false;
                modified = true;
                SetPhase(TE_SelectNoiseLayer);
            }
            else if (input == "n" || input == "b") {
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Seed: {
            StringToInt seed = ToInt(input);
            if (input == "r") {
                levelProperties_.noiseLayers[targetLayer_].seed = rand() % 10000;
                modified = true;
                SetPhase(TE_SelectProperty);
            }
            else if (seed.valid) {
                levelProperties_.noiseLayers[targetLayer_].seed = seed.value;
                modified = true;
                SetPhase(TE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Frequency: {
            StringToVec2 frequency = ToVec2(input, levelProperties_.noiseLayers[targetLayer_].frequency);
            StringToFloat uniformFrequency = ToFloat(input);
            if (frequency.valid) {
                levelProperties_.noiseLayers[targetLayer_].frequency = frequency.value;
                modified = true;
                SetPhase(TE_SelectProperty);
            }
            else if (uniformFrequency.valid) {
                levelProperties_.noiseLayers[targetLayer_].frequency.x = uniformFrequency.value;
                levelProperties_.noiseLayers[targetLayer_].frequency.y = uniformFrequency.value;
                modified = true;
                SetPhase(TE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Multiplier: {
            StringToFloat multiplier = ToFloat(input);
            if (multiplier.valid) {
                levelProperties_.noiseLayers[targetLayer_].multiplier = multiplier.value;
                modified = true;
                SetPhase(TE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Exponent: {
            StringToFloat exponent = ToFloat(input);
            if (exponent.valid) {
                levelProperties_.noiseLayers[targetLayer_].exponent = exponent.value;
                modified = true;
                SetPhase(TE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(TE_SelectProperty);
            }
            break;
        }
    }
    textInput_.ClearInput();

    if (modified) {
        terrain_.GenerateTerrainMap(
            levelProperties_.noiseLayers, 
            levelProperties_.blob,
            entityManager_.entities_,
            entityManager_.components_
        );
    }

    return exit;
}
