#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <glm/vec4.hpp>
bool GetBoolean(nlohmann::json& data, std::string property, bool defaultReturn = false);
int GetInt(nlohmann::json& data, std::string property, int defaultReturn = 0);
float GetFloat(nlohmann::json& data, std::string property, float defaultReturn = 0.0f);
glm::vec4 GetVec4(nlohmann::json& data, std::string propertry, glm::vec4 defaultReturn = glm::vec4(0.0f));
std::string GetString(nlohmann::json& data, std::string property, std::string defaultReturn = "");
