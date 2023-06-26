#pragma once
#include "RenderDefs.h"

struct MeshFileHeader {
    int numVertices;
    int numIndices;
};

struct Mesh {
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
};
