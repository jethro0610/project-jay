#include "LoadHelpers.h"
bool GetBoolean(nlohmann::json& data, std::string property, bool defaultReturn) {
    if (!data[property].is_boolean())
        return defaultReturn;

    return data[property].get<bool>();
}

int GetInt(nlohmann::json& data, std::string property, int defaultReturn) {
    if (!data[property].is_number())
        return defaultReturn;

    return data[property].get<int>();
}

float GetFloat(nlohmann::json& data, std::string property, float defaultReturn) {
    if (!data[property].is_number())
        return defaultReturn;

    return data[property].get<float>();
}

std::string GetString(nlohmann::json& data, std::string property, std::string defaultReturn) {
    if (!data[property].is_string())
        return defaultReturn;

    return data[property].get<std::string>();
}
