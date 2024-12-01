#pragma once
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Spread/SpreadType.h"
#include <array>

struct LevelProperties {
    Model* spreadModel;
    Model* weedModel_;
    std::array<Material, Model::MAX_MESHES_PER_MODEL> spreadMaterials[SpreadType_Num];

    Material terrainMaterial; 
    Material seedMaterial;
};
