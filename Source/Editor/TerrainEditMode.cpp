#include "TerrainEditMode.h"
#include "Level/LevelProperties.h"
#include "Terrain/Terrain.h"
#include "EditorTextInput.h"
#include "Helpers/StringConversion.h"
#include <GLFW/glfw3.h>

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
            std::string freq = std::to_string(levelProperties_.noiseLayers[targetLayer_].frequency);
            textInput_.SetLabel("Set Layer " + LayerName() + " Frequency (" + freq + "): "); 
            break;
        }

        case TE_Multiplier: {
            std::string mult = std::to_string(levelProperties_.noiseLayers[targetLayer_].multiplier);
            textInput_.SetLabel("Set Layer " + LayerName() + " Multiplier (" + mult + "): "); 
            break;
        }

        case TE_Exponent: {
            std::string exp = std::to_string(levelProperties_.noiseLayers[targetLayer_].exponent);
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
            break;
        }

        case TE_Activate: {
            if (input == "y") {
                levelProperties_.noiseLayers[targetLayer_].active = true;
                terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);
                SetPhase(TE_SelectProperty);
            }
            else if (input == "n") {
                SetPhase(TE_SelectNoiseLayer);
            }
            break;
        }

        case TE_Deactivate: {
            if (input == "y") {
                levelProperties_.noiseLayers[targetLayer_].active = false;
                terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);
                SetPhase(TE_SelectNoiseLayer);
            }
            else if (input == "n") {
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Seed: {
            StringToInt seed = ToInt(input);
            if (input == "r") {
                levelProperties_.noiseLayers[targetLayer_].seed = rand() % 10000;
                terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);
                SetPhase(TE_SelectProperty);
            }
            else if (seed.valid) {
                levelProperties_.noiseLayers[targetLayer_].seed = seed.value;
                terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Frequency: {
            StringToFloat frequency = ToFloat(input);
            if (frequency.valid) {
                levelProperties_.noiseLayers[targetLayer_].frequency = frequency.value;
                terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Multiplier: {
            StringToFloat multiplier = ToFloat(input);
            if (multiplier.valid) {
                levelProperties_.noiseLayers[targetLayer_].multiplier = multiplier.value;
                terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);
                SetPhase(TE_SelectProperty);
            }
            break;
        }

        case TE_Exponent: {
            StringToFloat exponent = ToFloat(input);
            if (exponent.valid) {
                levelProperties_.noiseLayers[targetLayer_].exponent = exponent.value;
                terrain_.GenerateTerrainMap(levelProperties_.noiseLayers, levelProperties_.blob);
                SetPhase(TE_SelectProperty);
            }
            break;
        }
    }
    textInput_.ClearInput();
    return false;
}
