#pragma once
#include "Texture.h"
#include "Shader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

enum TriangleType {
    ONE_SIDED,
    TWO_SIDED,
    TWO_SIDED_NEGATIVE_BACK
};

enum TranparencyType {
    TRANSPARENCY_NONE,
    TRANSPARENCY_UNORDERED,
    TRANSPARENCY_ADDITIVE,
    TRANSPARENCY_ADDITIVE_BRIGHT
};

struct MaterialProperties {
    glm::vec4 color; 

    glm::vec2 texScale;
    float specularPower;
    float specularThreshold;

    float specularBrightness;
    float fresnelPower;
    float fresnelScale;
    float fresnelBrightness;

    float variationFrequency;
    float variationMin;
    float variationMax;
    float variationPower;
        
    static MaterialProperties Default() {
        return {
            glm::vec4(1.0f),
            glm::vec2(1.0f),
            32.0f,
            0.3f,
            1.5f,
            4.0f,
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f
        };
    }
};

struct Material {
    static constexpr int MAX_TEXTURES = 8;
    static constexpr int MAX_PROPERTIES = 16;
    static constexpr int SHADOW_TEXINDEX = 14;
    static constexpr int TERRAIN_MAP_TEXINDEX = 15;

    Material() {
        shader = nullptr;
        shadowShader = nullptr;
        castShadows = false;
        transparencyType = TRANSPARENCY_NONE;
        triangleType = TriangleType::ONE_SIDED;

        numTextures = 0;
        for(int i = 0; i < MAX_TEXTURES; i++)
            textures[i] = nullptr;

        properties = {};
    }

    Shader* shader;
    Shader* shadowShader;

    int numTextures;
    Texture* textures[MAX_TEXTURES];
    bool castShadows;
    TranparencyType transparencyType;
    TriangleType triangleType;
    MaterialProperties properties;
};
