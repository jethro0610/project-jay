#pragma once
#include <glm/glm.hpp>

float RandomFloatRange(float min, float max);
glm::vec3 RandomVector(float maxDist);
glm::vec3 RandomVector(glm::vec3& min, glm::vec3& max);
glm::vec4 RandomVec4(glm::vec4& min, glm::vec4& max);
glm::vec2 RandomVector2D(float maxDist);
glm::vec3 RandomVectorHeight(float maxDist, float minHeight, float maxHeight);
