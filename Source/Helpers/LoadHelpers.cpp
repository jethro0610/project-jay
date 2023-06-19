#include "LoadHelpers.h"
bool GetBoolean(nlohmann::json& data, std::string property, bool defaultReturn) {
    if (!data[property].is_boolean())
        return defaultReturn;

    return data[property].get<bool>();
}

float GetFloat(nlohmann::json& data, std::string property, float defaultReturn) {
    if (!data[property].is_number())
        return defaultReturn;

    return data[property].get<float>();
}
