#include "TerrainEditMode.h"
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
            textInput_.SetLabel("Layer (0-3): ");
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
            std::string seed = std::to_string(terrain_.noiseLayers_[targetLayer_].seed_);
            textInput_.SetLabel("Set Layer " + LayerName() + " Seed (" + seed + "): "); 
            break;
        }

        case TE_Frequency: {
            std::string freq = std::to_string(terrain_.noiseLayers_[targetLayer_].frequency_);
            textInput_.SetLabel("Set Layer " + LayerName() + " Frequency (" + freq + "): "); 
            break;
        }

        case TE_Multiplier: {
            std::string mult = std::to_string(terrain_.noiseLayers_[targetLayer_].multiplier_);
            textInput_.SetLabel("Set Layer " + LayerName() + " Multiplier (" + mult + "): "); 
            break;
        }

        case TE_Exponent: {
            std::string exp = std::to_string(terrain_.noiseLayers_[targetLayer_].exponent_);
            textInput_.SetLabel("Set Layer " + LayerName() + " Exponent (" + exp + "): "); 
            break;
        }
    }
}

void TerrainEditMode::Update() {
    textInput_.ReadInput();
}

struct StringToFloat {
    bool valid;
    float value;
};

StringToFloat ToFloat(const std::string& str) {
    try {
        return {
            true,
            std::stof(str)
        };
    }
    catch(...) {
        return {
            false,
            0.0f
        };
    }
}

struct StringToInt {
    bool valid;
    int value;
};

StringToInt ToInt(const std::string& str) {
    try {
        return {
            true,
            std::stoi(str)
        };
    }
    catch(...) {
        return {
            false,
            0
        };
    }
}

bool TerrainEditMode::OnConfirm() {
    const std::string& input = textInput_.Get();
    switch(phase_) {
        case TE_SelectNoiseLayer: {
            StringToInt layer = ToInt(input);
            if (layer.valid && layer.value >= 0 && layer.value <= 3) {
                targetLayer_ = layer.value;
                if (terrain_.noiseLayers_[targetLayer_].active_)
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
                terrain_.noiseLayers_[targetLayer_].active_ = true;
                terrain_.GenerateHeightmap();
                resourceManager_.GenerateHeightmapTexture(terrain_);
                SetPhase(TE_SelectProperty);
            }
            else if (input == "n") {
                SetPhase(TE_SelectNoiseLayer);
            }
        }

        case TE_Deactivate: {
            if (input == "y") {
                terrain_.noiseLayers_[targetLayer_].active_ = false;
                terrain_.GenerateHeightmap();
                resourceManager_.GenerateHeightmapTexture(terrain_);
                SetPhase(TE_SelectNoiseLayer);
            }
            else if (input == "n") {
                SetPhase(TE_SelectProperty);
            }
        }

        case TE_Seed: {
            StringToInt seed = ToInt(input);
            if (input == "r") {
                terrain_.noiseLayers_[targetLayer_].seed_= rand() % 10000;
                terrain_.GenerateHeightmap();
                resourceManager_.GenerateHeightmapTexture(terrain_);
                SetPhase(TE_SelectProperty);
            }
            else if (seed.valid) {
                terrain_.noiseLayers_[targetLayer_].seed_ = seed.value;
                terrain_.GenerateHeightmap();
                resourceManager_.GenerateHeightmapTexture(terrain_);
                SetPhase(TE_SelectProperty);
            }
        }

        case TE_Frequency: {
            StringToFloat frequency = ToFloat(input);
            if (frequency.valid) {
                terrain_.noiseLayers_[targetLayer_].frequency_ = frequency.value;
                terrain_.GenerateHeightmap();
                resourceManager_.GenerateHeightmapTexture(terrain_);
                SetPhase(TE_SelectProperty);
            }
        }

        case TE_Multiplier: {
            StringToFloat multiplier = ToFloat(input);
            if (multiplier.valid) {
                terrain_.noiseLayers_[targetLayer_].multiplier_ = multiplier.value;
                terrain_.GenerateHeightmap();
                resourceManager_.GenerateHeightmapTexture(terrain_);
                SetPhase(TE_SelectProperty);
            }
        }

        case TE_Exponent: {
            StringToFloat exponent = ToFloat(input);
            if (exponent.valid) {
                terrain_.noiseLayers_[targetLayer_].exponent_ = exponent.value;
                terrain_.GenerateHeightmap();
                resourceManager_.GenerateHeightmapTexture(terrain_);
                SetPhase(TE_SelectProperty);
            }
        }
    }
    textInput_.ClearInput();
    return false;
}
