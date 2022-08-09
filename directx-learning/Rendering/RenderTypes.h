#pragma once
#include "../Transform.h"

struct PerObjectData {
    mat4 worldViewProj;
};

struct ColorVertex {
    vec3 pos;
    vec3 col;
};