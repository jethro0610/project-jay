#pragma once
#include "../Transform.h"

struct PerObjectData {
    mat4 worldViewProj;
};

struct Vertex {
    vec3 pos;
    vec2 uv;
};