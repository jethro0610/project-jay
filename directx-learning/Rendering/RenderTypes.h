#pragma once
#include "../Transform.h"

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

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec2 uv;
};