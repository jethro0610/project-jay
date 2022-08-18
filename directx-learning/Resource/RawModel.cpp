#include "RawModel.h"

int RawMesh::GetVertexByteWidth() {
    if (skeletal_)
        return vertexCount_ * (STATIC_VERTEX_SIZE + SKELETAL_VERTEX_INFO_SIZE);
    else
        return vertexCount_ * STATIC_VERTEX_SIZE;
}

int RawMesh::GetIndexByteWidth() {
    return indexCount_ * sizeof(uint16_t);
}