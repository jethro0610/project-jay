#pragma once
#include <string>
#include <vector>

struct EntityProperties {
    std::vector<std::pair<std::string, float*>> floats;
    std::vector<std::pair<std::string, int*>> ints;
    std::vector<std::pair<std::string, bool*>> bools;

    EntityProperties(
        std::vector<std::pair<std::string, float*>> floatProperties, 
        std::vector<std::pair<std::string, int*>> intProperties,
        std::vector<std::pair<std::string, bool*>> boolProperties 
    )  {
        floats = floatProperties;
        ints = intProperties;
        bools = boolProperties;
    }

    EntityProperties() {};
    float* GetFloat(const std::string& property);
    int* GetInt(const std::string& property);
    bool* GetBool(const std::string& property);
    std::string GetValueAsString(const std::string& property);
};
