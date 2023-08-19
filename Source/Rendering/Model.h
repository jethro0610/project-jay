#pragma once
#include <vector>
#include "Mesh.h"

struct ModelFileHeader {    
    int numMeshes;
    // int numJoints;
};  

struct Model {
    std::vector<Mesh> meshes;
};
