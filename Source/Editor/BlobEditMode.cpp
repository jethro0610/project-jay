#include "BlobEditMode.h"
#include "Entity/EntityManager.h"
#include "Level/LevelProperties.h"
#include "Terrain/Terrain.h"
#include "EditorLevel.h"
#include "EditorTextInput.h"
#include "Helpers/StringConversion.h"
#include <format>

BlobEditMode::BlobEditMode(EditorModeArgs args):
EditorMode(args) {

}

void BlobEditMode::OnStart() {
    EditorMode::OnStart();
    SetPhase(BE_SelectProperty);
}

void BlobEditMode::OnEnd() {
    textInput_.Clear();
}

void BlobEditMode::SetPhase(BlobEditPhase phase) {
    phase_ = phase;
    switch (phase_) {
        case BE_SelectProperty: {
            textInput_.SetLabel("Modify Blob (s/f/n/x): ");
            break;
        }

        case BE_Seed: {
            std::string seed = std::to_string(level_.properties_.blob.seed);
            textInput_.SetLabel("Set Blob Seed (" + seed + "): "); 
            break;
        }

        case BE_Frequency: {
            std::string freq = std::format("{:.3f}", level_.properties_.blob.frequency);
            textInput_.SetLabel("Set Blob Frequency (" + freq + "): "); 
            break;
        }

        case BE_MinRadius: {
            std::string minRadius = std::format("{:.3f}", level_.properties_.blob.minRadius);
            textInput_.SetLabel("Set Blob Min Radius(" + minRadius + "): "); 
            break;
        }

        case BE_MaxRadius: {
            std::string maxRadius = std::format("{:.3f}", level_.properties_.blob.maxRadius);
            textInput_.SetLabel("Set Blob Max Radius (" + maxRadius + "): "); 
            break;
        }
    }
}

void BlobEditMode::Update() {
    textInput_.ReadInput();
}

bool BlobEditMode::OnConfirm() {
    const std::string& input = textInput_.Get();
    bool modified = false;
    bool exit = false;
    switch(phase_) {
        case BE_SelectProperty: {
            if (input == "s")
                SetPhase(BE_Seed);
            else if (input == "f")
                SetPhase(BE_Frequency);
            else if (input == "n")
                SetPhase(BE_MinRadius);
            else if (input == "x")
                SetPhase(BE_MaxRadius);
            else if (input == "b")
                exit = true;
            break;
        }

        case BE_Seed: {
            StringToInt seed = ToInt(input);
            if (input == "r") {
                level_.properties_.blob.seed = rand() % 10000;
                modified = false;
                SetPhase(BE_SelectProperty);
            }
            else if (seed.valid) {
                level_.properties_.blob.seed  = seed.value;
                modified = false;
                SetPhase(BE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(BE_SelectProperty);
            }
            break;
        }

        case BE_Frequency: {
            StringToFloat frequency = ToFloat(input);
            if (frequency.valid) {
                level_.properties_.blob.frequency  = frequency.value;
                modified = false;
                SetPhase(BE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(BE_SelectProperty);
            }
            break;
        }

        case BE_MinRadius: {
            StringToFloat multiplier = ToFloat(input);
            if (multiplier.valid) {
                level_.properties_.blob.minRadius  = multiplier.value;
                modified = false;
                SetPhase(BE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(BE_SelectProperty);
            }
            break;
        }

        case BE_MaxRadius: {
            StringToFloat exponent = ToFloat(input);
            if (exponent.valid) {
                level_.properties_.blob.maxRadius  = exponent.value;
                modified = false;
                SetPhase(BE_SelectProperty);
            }
            else if (input == "b") {
                SetPhase(BE_SelectProperty);
            }
            break;
        }
    }
    textInput_.ClearInput();

    if (modified) {
        terrain_.GenerateTerrainMap(
            level_.properties_.noiseLayers,
            level_.properties_.blob,
            entityManager_.entities_,
            entityManager_.components_
        );
    }

    return exit;
}