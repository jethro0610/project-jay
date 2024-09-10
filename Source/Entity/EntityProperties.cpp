#include "EntityProperties.h"
#include "Logging/Logger.h"
#include <cstring>

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


void EntityProperties::AssignProperties(EntityPropertiesAssigner& assigner) {
    for (int i = 0; i < assigner.numFloats; i++)
        *GetFloat(assigner.floatStrings[i]) = assigner.floats[i]; 

    for (int i = 0; i < assigner.numInts; i++)
        *GetInt(assigner.intStrings[i]) = assigner.ints[i]; 

    for (int i = 0; i < assigner.numBools; i++)
        *GetInt(assigner.boolStrings[i]) = assigner.bools[i]; 
}

void EntityPropertiesAssigner::SetInt(const std::string& property, int value) {
    const char* prop = property.c_str();
    strcpy(intStrings[numInts], prop);
    ints[numInts] = value;
    numInts++;
}

void EntityPropertiesAssigner::SetFloat(const std::string& property, float value) {
    const char* prop = property.c_str();
    strcpy(floatStrings[numFloats], prop);
    floats[numFloats] = value;
    numFloats++;
}

void EntityPropertiesAssigner::SetBool(const std::string& property, bool value) {
    const char* prop = property.c_str();
    strcpy(boolStrings[numBools], prop);
    bools[numBools] = value;
    numBools++;
}
