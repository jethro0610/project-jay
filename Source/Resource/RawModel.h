#pragma once
#include <vector>
#include <glm.hpp>
using namespace glm;

const uint8_t MODEL_HEADER_SIZE = sizeof(uint8_t);
const uint8_t MESH_HEADER_SIZE = 2 * sizeof(uint16_t);

const uint8_t STATIC_VERTEX_SIZE = 4 * sizeof(vec3) + sizeof(vec2);
const uint8_t SKELETAL_VERTEX_INFO_SIZE = sizeof(ivec4) + sizeof(vec4);

class RawMesh {
public:
    bool skeletal_;
    uint16_t vertexCount_;
    void* vertexBuffer_;

    uint16_t indexCount_;
    void* indexBuffer_;

    int GetVertexByteWidth() const;
    int GetIndexByteWidth() const;
};

class RawModel {
public:
    RawModel(const char* modelName, bool skeletal = false);

    bool skeletal_;
    std::vector<unsigned char> rawBytes;
    std::vector<RawMesh> meshes_;
};

