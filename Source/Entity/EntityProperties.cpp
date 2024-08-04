#include "EntityProperties.h"

float* EntityProperties::GetFloat(const std::string& property) {
    for (auto& pair : floats) {
        if (pair.first == property)
            return pair.second;
    }
    return nullptr;
}

int* EntityProperties::GetInt(const std::string& property) {
    for (auto& pair : ints) {
        if (pair.first == property)
            return pair.second;
    }
    return nullptr;
}

bool* EntityProperties::GetBool(const std::string& property) {
    for (auto& pair : bools) {
        if (pair.first == property)
            return pair.second;
    }
    return nullptr;
}

std::string EntityProperties::GetValueAsString(const std::string& property) {
    float* floatValue = GetFloat(property);
    if (floatValue != nullptr)
        return std::to_string(*floatValue);

    int* intValue = GetInt(property);
    if (intValue != nullptr)
        return std::to_string(*intValue);

    bool* boolValue = GetBool(property);
    if (boolValue != nullptr) {
        if (*boolValue)
            return "t";
        else
            return "f";
    }

    return "NULL";
}
