#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <unordered_set>
#include "../../Types/FixedVector.h"
#include "TerrainModifier.h"

class FastNoiseLite;

class World {
public:
    World(FastNoiseLite& noise);

    float GetHeight(glm::vec2 position) const;
    float GetHeight(glm::vec3 position) const;

    glm::vec3 GetNormal(glm::vec2 position, float epsilon = 1.0f) const;
    glm::vec3 GetNormal(glm::vec3 position, float epsilon = 1.0f) const;

    float minRadius_;
    float maxRadius_;
    float edgeJaggedness_;
    float edgeFalloff_;
    float edgePower_;

private:
    FastNoiseLite& noise_;
};
