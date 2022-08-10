#pragma once
#include <vector>

#define MODEL_HEADER_SIZE sizeof(uint8_t)
#define SECTION_HEADER_SIZE 2 * sizeof(uint16_t)

class RawMeshSection {
public:
    uint16_t vertexCount_;
    void* vertexBuffer_;

    uint16_t indexCount_;
    void* indexBuffer_;

    int GetVertexByteWidth();
    int GetIndexByteWidth();
};

class RawMesh {
public:
    RawMesh(const char* meshName);
    std::vector<unsigned char> rawBytes;
    std::vector<RawMeshSection> sections_;
};

