#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

bool GetBoolean(nlohmann::json& data, std::string property, bool defaultReturn = false);
int GetInt(nlohmann::json& data, std::string property, int defaultReturn = 0);
float GetFloat(nlohmann::json& data, std::string property, float defaultReturn = 0.0f);

glm::vec4 GetVec4(nlohmann::json& data, glm::vec4 defaultReturn = glm::vec4(0.0f));
glm::vec4 GetVec4(nlohmann::json& data, std::string propertry, glm::vec4 defaultReturn = glm::vec4(0.0f));

glm::vec3 GetVec3(nlohmann::json& data, glm::vec3 defaultReturn = glm::vec3(0.0f));
glm::vec3 GetVec3(nlohmann::json& data, std::string property, glm::vec3 defaultReturn = glm::vec3(0.0f));

std::string GetString(nlohmann::json& data, std::string property, std::string defaultReturn = "");
