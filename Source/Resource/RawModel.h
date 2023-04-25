#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

const uint8_t MODEL_HEADER_SIZE = sizeof(uint8_t);
const uint8_t MESH_HEADER_SIZE = 2 * sizeof(uint16_t);

const uint8_t STATIC_VERTEX_SIZE = 4 * sizeof(glm::vec3) + sizeof(glm::vec2);
const uint8_t SKELETAL_VERTEX_INFO_SIZE = sizeof(glm::ivec4) + sizeof(glm::vec4);

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

