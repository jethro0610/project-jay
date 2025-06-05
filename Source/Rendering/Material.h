#pragma once
#include "Texture.h"
#include "Shader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

enum TriangleType {
    ONE_SIDED,
    TWO_SIDED,
    TWO_SIDED_NEGATIVE_BACK,
    BACKFACE
};

enum TranparencyType {
    TRANSPARENCY_NONE,
    TRANSPARENCY_UNORDERED,
    TRANSPARENCY_ADDITIVE,
    TRANSPARENCY_ADDITIVE_BRIGHT
};

struct BaseMaterialProprties {
    glm::vec4 color;
    glm::vec2 texScale;
};

struct SpecularFresnelProperties : BaseMaterialProprties {
    float specularPower;
    float specularThreshold;

    float specularBrightness;
    float fresnelPower;
    float fresnelScale;
    float fresnelBrightness;

    static SpecularFresnelProperties Default() {
        return {
            glm::vec4(1.0f),
            glm::vec2(1.0f),
            32.0f,
            0.3f,
            1.5f,
            4.0f,
            1.0f,
            1.0f,
        };
    }
};

struct VariationProperties : SpecularFresnelProperties {
    float variationFrequency;
    float variationMin;
    float variationMax;
    float variationPower;
    
    static VariationProperties Default() {
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
            1.0f,
        };
    };
};

struct LightVolumeProperties {
    glm::vec3 origin;
    float radius;
    float stepDistance;
    float accumulationStrength;
    float falloffHeight;
    float padding;
    glm::vec4 color;

    static LightVolumeProperties Default() {
        return {
            glm::vec3(0.0f),
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            glm::vec4(1.0f)
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
        volumetric = false;

        numTextures = 0;
        for(int i = 0; i < MAX_TEXTURES; i++)
            textures[i] = nullptr;

        padding[0] = {};
        padding[1] = {};
        padding[2] = {};
        padding[3] = {};
    }

    Shader* shader;
    Shader* shadowShader;

    int numTextures;
    Texture* textures[MAX_TEXTURES];
    bool castShadows;
    TranparencyType transparencyType;
    TriangleType triangleType;
    bool volumetric;

    union {
        BaseMaterialProprties properties;
        SpecularFresnelProperties specularProperties;
        VariationProperties variationProperties;
        LightVolumeProperties lightVolumeProperties;
        glm::vec4 padding[4];
    };
};
