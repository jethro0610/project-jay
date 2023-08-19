#pragma once
#include "Mesh.h"

const int MAX_MESHES_PER_MODEL = 12;

struct ModelFileHeader {    
    int numMeshes;
    // int numJoints;
};  

struct Model {
    Mesh meshes[MAX_MESHES_PER_MODEL];
    int numMeshes;
};
