#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct PerFrameData {
    glm::vec3 cameraPos;
    float time;
    float aspectRatio;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    uint32_t pad0;
};

struct PerObjectData {
    glm::mat4x4 worldMat;
    glm::mat4x4 worldViewProj;
    glm::mat4x4 normalMat;
};

struct PerSkeletonData {
    glm::mat4x4 jointMatrices[32];
};

struct PerChunkData {
    glm::vec4 chunkPos;
    uint32_t modifierCount;

    uint32_t pad0;
    uint32_t pad1;
    uint32_t pad2;
};

struct PerUIData {
    float spreadMeter;
    glm::vec3 pad0;
};

struct StaticVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 uv;
};

struct SkeletalVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 uv;
    glm::ivec4 joints;
    glm::vec4 weights;
};

struct WorldVertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct WorldTri {
    WorldVertex v0;
    WorldVertex v1;
    WorldVertex v2;
};

struct InstanceData {
    glm::vec3 position;
};

struct TextData {
    uint32_t character;
};
