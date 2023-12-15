#pragma once
#include "Mesh.h"
#include "Debug/DebugName.h"
#include <vector_const.h>

struct ModelFileHeader {    
    int numMeshes;
    int numAnimations;
    bool skeletal;
};  

struct Model {
    static const int MAX_MESHES_PER_MODEL = 24;
    Model() {
        meshes = {};
    }
    vector_const<Mesh, MAX_MESHES_PER_MODEL> meshes;

    USE_DEBUG_NAME;
};
