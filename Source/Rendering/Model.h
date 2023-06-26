#pragma once
#include "Mesh.h"

const uint8_t MAX_MESHES = 8;

struct ModelFileHeader {
    int numMeshes;
};

struct Model {
    Mesh meshes[MAX_MESHES];
    uint8_t numMeshes;
};
