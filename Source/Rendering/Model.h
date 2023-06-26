#pragma once
#include "Mesh.h"

const uint8_t MAX_MESHES_PER_MODEL = 8;

struct ModelFileHeader {
    int numMeshes;
};

struct Model {
    Mesh meshes[MAX_MESHES_PER_MODEL];
    uint8_t numMeshes;
};
