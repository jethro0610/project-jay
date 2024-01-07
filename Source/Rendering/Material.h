#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Shared_MProps.h"

enum TriangleType {
    ONE_SIDED,
    TWO_SIDED,
    TWO_SIDED_NEGATIVE_BACK
};

struct Material {
    static constexpr int MAX_TEXTURES = 8;
    static constexpr int MAX_PROPERTIES = 16;
    static constexpr int SHADOW_TEXINDEX = 14;
    static constexpr int TERRAIN_MAP_TEXINDEX = 15;

    Material() {
        shader = nullptr;
        shader = shadowShader;
        castShadows = false;
        triangleType = TriangleType::ONE_SIDED;

        numTextures = 0;
        for(int i = 0; i < MAX_TEXTURES; i++)
            textures[i] = nullptr;

        for (int i = 0; i < MAX_PROPERTIES; i++)
            properties[i] = 0.0f;
    }

    Shader* shader;
    Shader* shadowShader;

    int numTextures;
    Texture* textures[MAX_TEXTURES];
    bool castShadows;
    TriangleType triangleType;
    float properties[MAX_PROPERTIES];
};
