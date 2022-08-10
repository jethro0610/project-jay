#include "RawMesh.h"
#include <fstream>
#include <assert.h>

RawMesh::RawMesh(const char* filename) {
    std::ifstream infile;
    infile.open(filename);
    assert(infile.is_open());

    // Convert the file stream to a byte vector
    rawBytes = std::vector<unsigned char>((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    infile.close();

    vertexCount_ = rawBytes[0];
    indexCount_ = rawBytes[1];

    vertexBuffer_ = &rawBytes[2];
    indexBuffer_ = &rawBytes[2 + GetVertexByteWidth()];
}

int RawMesh::GetVertexByteWidth() {
    return vertexCount_ * 3 * sizeof(float);
}

int RawMesh::GetIndexByteWidth() {
    return indexCount_ * sizeof(uint16_t);
}