#pragma once
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include <vector_const.h>

struct LevelProperties {
    Model* spreadModel;
    vector_const<Material*, Model::MAX_MESHES_PER_MODEL> spreadMaterials;

    Material* terrainMaterial; 
    Material* seedMaterial;
};
