#pragma once
#include <json.hpp>
bool GetBoolean(nlohmann::json& data, std::string property, bool defaultReturn = false);
float GetFloat(nlohmann::json& data, std::string property, float defaultReturn = 0.0f);
