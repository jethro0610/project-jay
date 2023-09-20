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

glm::vec4 GetVec4(nlohmann::json& data, glm::vec4 defaultReturn) {
    float x = GetFloat(data, "x", defaultReturn.x);
    float y = GetFloat(data, "y", defaultReturn.y);
    float z = GetFloat(data, "z", defaultReturn.z);
    float w = GetFloat(data, "w", defaultReturn.w);

    return glm::vec4(x, y, z, w);
}

glm::vec4 GetVec4(nlohmann::json& data, std::string property, glm::vec4 defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto& vector = data[property];
    return GetVec4(vector, defaultReturn);
}

glm::vec3 GetVec3(nlohmann::json& data, glm::vec3 defaultReturn) {
    float x = GetFloat(data, "x", defaultReturn.x);
    float y = GetFloat(data, "y", defaultReturn.y);
    float z = GetFloat(data, "z", defaultReturn.z);

    return glm::vec3(x, y, z);
}

glm::vec3 GetVec3(nlohmann::json& data, std::string property, glm::vec3 defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto& vector = data[property];
    return GetVec3(vector, defaultReturn);
}
