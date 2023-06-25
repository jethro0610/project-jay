#include "Mesh.h"

struct ModelFileHeader {
    int numMeshes;
};

struct Model {
    Mesh meshes[8];
    uint8_t numMeshes;
};
