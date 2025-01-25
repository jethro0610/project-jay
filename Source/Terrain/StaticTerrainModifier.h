#pragma once
#include <glm/vec2.hpp>

class StaticTerrainModifier {
    virtual bool InRange(const glm::vec2& pos) = 0;
    virtual float GetHeight(const glm::vec2& pos) = 0;
};
