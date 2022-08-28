#pragma once
#include <string>
#include <vector>

struct Material {
    std::string vertexShaderName;
    std::string pixelShaderName;
    std::string textureNames[4];
};