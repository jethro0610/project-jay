#pragma once
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include <array>

struct LevelProperties {
    Model* spreadModel;
    std::array<Material, Model::MAX_MESHES_PER_MODEL> spreadMaterials;

    Material terrainMaterial; 
    Material seedMaterial;
};
