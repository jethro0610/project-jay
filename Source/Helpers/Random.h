#pragma once
#include <glm/glm.hpp>

float RandomFloatRange(float min, float max);
glm::vec3 RandomVector(float maxDist);
glm::vec3 RandomVector(const glm::vec3& min, const glm::vec3& max);
glm::vec3 RandomVector(float min, float max);
glm::vec4 RandomVec4(const glm::vec4& min, const glm::vec4& max);
glm::vec3 RandomVectorPlanar(float maxDist);
glm::vec3 RandomVectorPlanar(float minDist, float maxDist);
glm::vec2 RandomVector2D(float maxDist);
glm::vec2 RandomVector2D(float minDist, float maxDist);
glm::vec3 RandomVectorHeight(float maxDist, float minHeight, float maxHeight);
