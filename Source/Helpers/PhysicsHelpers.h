#pragma once
#include <glm/vec3.hpp>

float GetAirtime(float gravity, float yVel, float curHeight, float targetHeight);
glm::vec3 GetProjectileVelocityToPoint(const glm::vec3& position, const glm::vec3& target, float gravity, float yVel);
