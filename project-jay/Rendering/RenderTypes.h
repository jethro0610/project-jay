#pragma once
#include "../Types/Transform.h"

struct DirectionalLight {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 direction;
};

struct PerObjectData {
    mat4 worldViewProj;
    mat4 normalMat;
};

struct PerSkeletonData {
    mat4 jointMatrices[32];
};

struct StaticVertex {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec2 uv;
};

struct SkeletalVertex {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec2 uv;
    ivec4 joints;
    vec4 weights;
};

struct WorldVertex {
    vec3 position;
    vec3 normal;
};