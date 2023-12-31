#include "LoadHelpers.h"
bool GetBoolean(nlohmann::json& data, const std::string& property, bool defaultReturn) {
    if (!data[property].is_boolean())
        return defaultReturn;

    return data[property].get<bool>();
}

int GetInt(nlohmann::json& data, const std::string& property, int defaultReturn) {
    if (!data[property].is_number())
        return defaultReturn;

    return data[property].get<int>();
}

float GetFloat(nlohmann::json& data, const std::string& property, float defaultReturn) {
    if (!data[property].is_number())
        return defaultReturn;

    return data[property].get<float>();
}

std::string GetString(nlohmann::json& data, const std::string& property, const std::string& defaultReturn) {
    if (!data[property].is_string())
        return defaultReturn;

    return data[property].get<std::string>();
}

glm::quat GetQuat(nlohmann::json& data, const glm::quat& defaultReturn) {
    float x = GetFloat(data, "x", defaultReturn.x);
    float y = GetFloat(data, "y", defaultReturn.y);
    float z = GetFloat(data, "z", defaultReturn.z);
    float w = GetFloat(data, "w", defaultReturn.w);

    return glm::quat(x, y, z, w);
}

glm::quat GetQuat(nlohmann::json& data, const std::string& property, const glm::quat& defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto& vector = data[property];
    return GetQuat(vector, defaultReturn);
}

glm::vec4 GetVec4(nlohmann::json& data, const glm::vec4& defaultReturn) {
    float x = GetFloat(data, "x", defaultReturn.x);
    float y = GetFloat(data, "y", defaultReturn.y);
    float z = GetFloat(data, "z", defaultReturn.z);
    float w = GetFloat(data, "w", defaultReturn.w);

    return glm::vec4(x, y, z, w);
}

glm::vec4 GetVec4(nlohmann::json& data, const std::string& property, const glm::vec4& defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto& vector = data[property];
    return GetVec4(vector, defaultReturn);
}

glm::vec3 GetVec3(nlohmann::json& data, const glm::vec3& defaultReturn) {
    float x = GetFloat(data, "x", defaultReturn.x);
    float y = GetFloat(data, "y", defaultReturn.y);
    float z = GetFloat(data, "z", defaultReturn.z);

    return glm::vec3(x, y, z);
}

glm::vec3 GetVec3(nlohmann::json& data, const std::string& property, const glm::vec3& defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto& vector = data[property];
    return GetVec3(vector, defaultReturn);
}

glm::vec2 GetVec2(nlohmann::json& data, const glm::vec2& defaultReturn) {
    float x = GetFloat(data, "x", defaultReturn.x);
    float y = GetFloat(data, "y", defaultReturn.y);

    return glm::vec2(x, y);
}

glm::vec2 GetVec2(nlohmann::json& data, const std::string& property, const glm::vec2& defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto& vector = data[property];
    return GetVec2(vector, defaultReturn);
}

Transform GetTransform(nlohmann::json& data, const std::string& property, const Transform& defaultReturn) {
    if (!data.contains(property))
        return defaultReturn;

    auto& transformData = data[property];
    Transform returnTransform;

    returnTransform.position = GetVec3(transformData, "position", glm::vec3(0.0f));
    returnTransform.scale = GetVec3(transformData, "scale", glm::vec3(1.0f, 1.0f, 1.0f));
    returnTransform.rotation = glm::quat(GetVec3(transformData, "rotation", glm::vec3(0.0f)));

    return returnTransform;
}
