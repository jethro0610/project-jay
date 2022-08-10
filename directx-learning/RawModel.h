#pragma once
#include <vector>

#define MODEL_HEADER_SIZE sizeof(uint8_t)
#define MESH_HEADER_SIZE 2 * sizeof(uint16_t)

class RawMesh {
public:
    uint16_t vertexCount_;
    void* vertexBuffer_;

    uint16_t indexCount_;
    void* indexBuffer_;

    int GetVertexByteWidth();
    int GetIndexByteWidth();
};

class RawModel {
public:
    RawModel(const char* modelName);
    std::vector<unsigned char> rawBytes;
    std::vector<RawMesh> meshes_;
};

