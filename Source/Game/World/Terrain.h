#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <unordered_set>
#include "../../Types/Noise.h"
#include "Shared_TerrainProperties.h"
#include "TerrainModifier.h"

class Terrain {
public:
    Terrain(Noise& noise);

    glm::vec2 GetDistance(const glm::vec2& position, NoiseAccuracy accuracy = NA_Normal) const;
    glm::vec2 GetDistance(const glm::vec3& position, NoiseAccuracy accuracy = NA_Normal) const;

    float GetHeight(const glm::vec2& position, NoiseAccuracy accuracy = NA_Normal) const;
    float GetHeight(const glm::vec3& position, NoiseAccuracy accuracy = NA_Normal) const;

    glm::vec3 GetNormal(const glm::vec2& position, NoiseAccuracy accuracy = NA_Normal) const;
    glm::vec3 GetNormal(const glm::vec3& position, NoiseAccuracy accuracy = NA_Normal) const;

    TerrainProperties properties_;

private:
    Noise& noise_;
};
