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

glm::vec4 GetVec4(nlohmann::json& data, std::string property, glm::vec4 defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto vector = data[property];
    float x = GetFloat(vector, "x", 0.0f);
    float y = GetFloat(vector, "y", 0.0f);
    float z = GetFloat(vector, "z", 0.0f);
    float w = GetFloat(vector, "w", 1.0f);

    return glm::vec4(x, y, z, w);
}
