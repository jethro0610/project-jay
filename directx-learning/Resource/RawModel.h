#pragma once
#include <vector>
#include <glm.hpp>
using namespace glm;

#define MODEL_HEADER_SIZE sizeof(uint8_t)
#define MESH_HEADER_SIZE 2 * sizeof(uint16_t)

#define STATIC_VERTEX_SIZE (4 * sizeof(vec3) + sizeof(vec2))
#define SKELETAL_VERTEX_INFO_SIZE (sizeof(ivec4) + sizeof(vec4))

class RawMesh {
public:
    bool skeletal_;
    uint16_t vertexCount_;
    void* vertexBuffer_;

    uint16_t indexCount_;
    void* indexBuffer_;

    int GetVertexByteWidth();
    int GetIndexByteWidth();
};

class RawModel {
public:
    RawModel(const char* modelName, bool skeletal = false);

    bool skeletal_;
    std::vector<unsigned char> rawBytes;
    std::vector<RawMesh> meshes_;
};

