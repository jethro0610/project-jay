#pragma once
#include <vector_const.h>
#include "Mesh.h"

struct ModelFileHeader {    
    int numMeshes;
    int numAnimations;
    bool skeletal;
};  

struct Model {
    static const int MAX_MESHES_PER_MODEL = 24;
    vector_const<Mesh, MAX_MESHES_PER_MODEL> meshes;
};
