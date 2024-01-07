#pragma once
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Terrain/BlobProperties.h"
#include "Terrain/NoiseLayer.h"
#include <vector_const.h>

struct LevelProperties {
    Model* spreadModel;
    Material spreadMaterials[Model::MAX_MESHES_PER_MODEL];

    Material terrainMaterial; 
    Material seedMaterial;

    NoiseLayer noiseLayers[NoiseLayer::MAX];
    BlobProperties blob;
};
