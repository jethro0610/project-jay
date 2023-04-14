#pragma once
#include <json.hpp>
bool GetBoolean(nlohmann::json& data, std::string property, bool defaultReturn = false);
