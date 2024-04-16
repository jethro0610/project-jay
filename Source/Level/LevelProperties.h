#pragma once
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include <array>

struct LevelProperties {
    Model* spreadModel;
    Model* weedModel_;
    std::array<Material, Model::MAX_MESHES_PER_MODEL> spreadMaterials;
    std::array<Material, Model::MAX_MESHES_PER_MODEL> weedMaterials;

    Material terrainMaterial; 
    Material seedMaterial;
};
