#pragma once
#include <vector>

class RawMesh {
public:
    RawMesh(const char* meshName);
    std::vector<unsigned char> rawBytes;

    uint8_t vertexCount_;
    uint8_t indexCount_;

    void* vertexBuffer_;
    void* indexBuffer_;

    int GetVertexByteWidth();
    int GetIndexByteWidth();
};

