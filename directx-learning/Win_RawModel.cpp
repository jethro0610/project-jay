#include "RawModel.h"
#include <fstream>
#include <assert.h>

RawModel::RawModel(const char* modelName) {
    std::ifstream infile;
    infile.open(modelName, std::ios::in | std::ios::binary);
    assert(infile.is_open());

    // Convert the file stream to a byte vector
    
    infile.seekg(0, std::ios::end);
    int bytesSize = infile.tellg();
    rawBytes.reserve(bytesSize);
    infile.seekg(0, std::ios::beg);
    std::copy((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>(), std::back_inserter(rawBytes));
    infile.close();

    int meshCount = (uint8_t)rawBytes.data()[0];

    int currentBufferLocation = MODEL_HEADER_SIZE + meshCount * MESH_HEADER_SIZE;
    for (int i = 0; i < meshCount; i++) {
        RawMesh mesh = {};
        uint16_t* meshHeader = (uint16_t*)&rawBytes.data()[MODEL_HEADER_SIZE + i * MESH_HEADER_SIZE];
        mesh.vertexCount_ = meshHeader[0];
        mesh.indexCount_ = meshHeader[1];
        
        mesh.vertexBuffer_ = &rawBytes[currentBufferLocation];
        currentBufferLocation += mesh.GetVertexByteWidth();
        mesh.indexBuffer_ = &rawBytes[currentBufferLocation];
        currentBufferLocation += mesh.GetIndexByteWidth();
        
        meshes_.push_back(mesh);
    }
}

int RawMesh::GetVertexByteWidth() {
    return vertexCount_ * 11 * sizeof(float) ;
}

int RawMesh::GetIndexByteWidth() {
    return indexCount_ * sizeof(uint16_t);
}