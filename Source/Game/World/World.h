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
    World();

    glm::vec2 GetDistance(glm::vec2 position) const;
    glm::vec2 GetDistance(glm::vec3 position) const;
    float GetHeight(glm::vec2 position) const;
    float GetHeight(glm::vec3 position) const;

    glm::vec3 GetNormal(glm::vec2 position) const;
    glm::vec3 GetNormal(glm::vec3 position) const;

    WorldProperties properties_;

private:
    Noise noise_;
};
