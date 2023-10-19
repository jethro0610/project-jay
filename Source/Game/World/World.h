#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <unordered_set>
#include "../../Types/Noise.h"
#include "Shared_WorldProperties.h"
#include "TerrainModifier.h"

class World {
public:
    World(Noise& noise);

    glm::vec2 GetDistance(const glm::vec2& position) const;
    glm::vec2 GetDistanceFast(const glm::vec2& position) const;
    glm::vec2 GetDistance(const glm::vec3& position) const;
    glm::vec2 GetDistanceFast(const glm::vec3& position) const;

    float GetHeight(const glm::vec2& position) const;
    float GetHeightFast(const glm::vec2& position) const;
    float GetHeight(const glm::vec3& position) const;
    float GetHeightFast(const glm::vec3& position) const;

    glm::vec3 GetNormal(const glm::vec2& position) const;
    glm::vec3 GetNormal(const glm::vec3& position) const;

    WorldProperties properties_;

private:
    Noise& noise_;
};
