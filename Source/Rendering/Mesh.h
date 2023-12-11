#pragma once
#include "RenderDefs.h"

struct MeshFileHeader {
    int numVertices;
    int numIndices;
};

struct Mesh {
    Mesh() {
        vertexBuffer = BGFX_INVALID_HANDLE;
        indexBuffer = BGFX_INVALID_HANDLE;
    }
    VertexBufferHandle vertexBuffer;
    IndexBufferHandle indexBuffer;
};
