#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class EntityList;

class WaterManager {
public:
    WaterManager(EntityList& entityList);

    void Wet(const glm::vec2& position, float area);
    void Wet(const glm::vec3& position, float area);
private:
    EntityList& entities_;
};
