#pragma once
#include <glm.hpp>

glm::ivec3 WorldPositionToChunk(glm::vec3 position);
glm::ivec2 WorldPositionToChunk2D(glm::vec2 position);
glm::ivec3 GetNormalizedChunk(glm::ivec3 chunk);
glm::ivec2 GetNormalizedChunk2D(glm::ivec2 chunk);
