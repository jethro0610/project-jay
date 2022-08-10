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

    int sectionCount = (uint8_t)rawBytes.data()[0];

    int currentBufferLocation = MODEL_HEADER_SIZE + sectionCount * SECTION_HEADER_SIZE;
    for (int i = 0; i < sectionCount; i++) {
        RawMeshSection section = {};
        uint16_t* sectionHeader = (uint16_t*)&rawBytes.data()[MODEL_HEADER_SIZE + i * SECTION_HEADER_SIZE];
        section.vertexCount_ = sectionHeader[0];
        section.indexCount_ = sectionHeader[1];

        section.vertexBuffer_ = &rawBytes[currentBufferLocation];
        currentBufferLocation += section.GetVertexByteWidth();
        section.indexBuffer_ = &rawBytes[currentBufferLocation];
        currentBufferLocation += section.GetIndexByteWidth();
        sections_.push_back(section);
    }
}

int RawMeshSection::GetVertexByteWidth() {
    return vertexCount_ * 3 * sizeof(float);
}

int RawMeshSection::GetIndexByteWidth() {
    return indexCount_ * sizeof(uint16_t);
}