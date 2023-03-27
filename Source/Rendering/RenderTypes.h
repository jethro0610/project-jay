#pragma once
#include <glm.hpp>

struct PerFrameData {
    glm::vec3 cameraPos;
	float time;
    float aspectRatio;
    glm::vec3 extra;
};

struct PerObjectData {
    glm::mat4 worldMat;
    glm::mat4 worldViewProj;
    glm::mat4 normalMat;
};

struct PerSkeletonData {
    glm::mat4 jointMatrices[32];
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

struct InstanceData {
    glm::vec3 position;
};

struct TextData {
    uint32_t character;
};
