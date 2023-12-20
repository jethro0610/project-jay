#include "BlobEditMode.h"
#include "Helpers/StringConversion.h"

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
            std::string seed = std::to_string(terrain_.blobProperties_.seed);
            textInput_.SetLabel("Set Blob Seed (" + seed + "): "); 
            break;
        }

        case BE_Frequency: {
            std::string freq = std::to_string(terrain_.blobProperties_.frequency);
            textInput_.SetLabel("Set Blob Frequency (" + freq + "): "); 
            break;
        }

        case BE_MinRadius: {
            std::string minRadius = std::to_string(terrain_.blobProperties_.minRadius);
            textInput_.SetLabel("Set Blob Min Radius(" + minRadius + "): "); 
            break;
        }

        case BE_MaxRadius: {
            std::string maxRadius = std::to_string(terrain_.blobProperties_.maxRadius);
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
            break;
        }

        case BE_Seed: {
            StringToInt seed = ToInt(input);
            if (input == "r") {
                terrain_.blobProperties_.seed = rand() % 10000;
                terrain_.GenerateTerrainMap();
                resourceManager_.GenerateTerrainMapTexture(terrain_);
                SetPhase(BE_SelectProperty);
            }
            else if (seed.valid) {
                terrain_.blobProperties_.seed  = seed.value;
                terrain_.GenerateTerrainMap();
                resourceManager_.GenerateTerrainMapTexture(terrain_);
                SetPhase(BE_SelectProperty);
            }
            break;
        }

        case BE_Frequency: {
            StringToFloat frequency = ToFloat(input);
            if (frequency.valid) {
                terrain_.blobProperties_.frequency  = frequency.value;
                terrain_.GenerateTerrainMap();
                resourceManager_.GenerateTerrainMapTexture(terrain_);
                SetPhase(BE_SelectProperty);
            }
            break;
        }

        case BE_MinRadius: {
            StringToFloat multiplier = ToFloat(input);
            if (multiplier.valid) {
                terrain_.blobProperties_.minRadius  = multiplier.value;
                terrain_.GenerateTerrainMap();
                resourceManager_.GenerateTerrainMapTexture(terrain_);
                SetPhase(BE_SelectProperty);
            }
            break;
        }

        case BE_MaxRadius: {
            StringToFloat exponent = ToFloat(input);
            if (exponent.valid) {
                terrain_.blobProperties_.maxRadius  = exponent.value;
                terrain_.GenerateTerrainMap();
                resourceManager_.GenerateTerrainMapTexture(terrain_);
                SetPhase(BE_SelectProperty);
            }
            break;
        }
    }
    textInput_.ClearInput();
    return false;
}
