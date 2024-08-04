#pragma once
#include <string>
#include <vector>

struct EntityProperties {
    std::vector<std::pair<std::string, float*>> floats;
    std::vector<std::pair<std::string, int*>> ints;

    EntityProperties(
        std::vector<std::pair<std::string, float*>> propFloats, 
        std::vector<std::pair<std::string, int*>> propInts 
    )  {
        floats = propFloats;
        ints = propInts;
    }

    EntityProperties() {};
    int* GetInt(const std::string& property);
    float* GetFloat(const std::string& property);
    std::string GetValueAsString(const std::string& property);
};
