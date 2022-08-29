#include "RawModel.h"

int RawMesh::GetVertexByteWidth() const {
    if (skeletal_)
        return vertexCount_ * (STATIC_VERTEX_SIZE + SKELETAL_VERTEX_INFO_SIZE);
    else
        return vertexCount_ * STATIC_VERTEX_SIZE;
}

int RawMesh::GetIndexByteWidth() const {
    return indexCount_ * sizeof(uint16_t);
}