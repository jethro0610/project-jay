#pragma once
#include <glm.hpp>

glm::ivec3 GetChunkAtWorldPosition(glm::vec3 position);
glm::ivec2 GetChunkAtWorldPosition2D(glm::vec2 position);
glm::ivec3 GetNormalizedChunk(glm::ivec3 chunk);
glm::ivec2 GetNormalizedChunk2D(glm::ivec2 chunk);
