#pragma once
#include <glm/glm.hpp>

float RandomFloatRange(float min, float max);
glm::vec3 RandomVector(float maxDist);
glm::vec2 RandomVector2D(float maxDist);
glm::vec3 RandomVectorHeight(float maxDist, float minHeight, float maxHeight);
