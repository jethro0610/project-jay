#include "EntityProperties.h"

int* EntityProperties::GetInt(const std::string& property) {
    for (auto& pair : ints) {
        if (pair.first == property)
            return pair.second;
    }
    return nullptr;
}

float* EntityProperties::GetFloat(const std::string& property) {
    for (auto& pair : floats) {
        if (pair.first == property)
            return pair.second;
    }
    return nullptr;
}

std::string EntityProperties::GetValueAsString(const std::string& property) {
    int* intValue = GetInt(property);
    if (intValue != nullptr)
        return std::to_string(*intValue);

    float* floatValue = GetFloat(property);
    if (floatValue != nullptr)
        return std::to_string(*floatValue);

    return "NULL";
}
