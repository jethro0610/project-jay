#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Types/Transform.h"

bool GetBoolean(nlohmann::json& data, const std::string& property, bool defaultReturn = false);
int GetInt(nlohmann::json& data, const std::string& property, int defaultReturn = 0);
float GetFloat(nlohmann::json& data, const std::string& property, float defaultReturn = 0.0f);

glm::quat GetQuat(nlohmann::json& data, const glm::vec4& defaultReturn = glm::vec4(0.0f));
glm::quat GetQuat(nlohmann::json& data, const std::string& propertry, const glm::vec4& defaultReturn = glm::vec4(0.0f));

glm::vec4 GetVec4(nlohmann::json& data, const glm::vec4& defaultReturn = glm::vec4(0.0f));
glm::vec4 GetVec4(nlohmann::json& data, const std::string& propertry, const glm::vec4& defaultReturn = glm::vec4(0.0f));

glm::vec3 GetVec3(nlohmann::json& data, const glm::vec3& defaultReturn = glm::vec3(0.0f));
glm::vec3 GetVec3(nlohmann::json& data, const std::string& property, const glm::vec3& defaultReturn = glm::vec3(0.0f));

Transform GetTransform(nlohmann::json& data, const std::string& property, const Transform& defaultReturn = Transform());

std::string GetString(nlohmann::json& data, const std::string& property, const std::string& defaultReturn = "");
