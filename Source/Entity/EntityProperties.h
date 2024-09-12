#pragma once
#include <string>
#include <vector>
#include <vector_const.h>
class Entity;
class EntityPropertiesAssigner;

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

    void AssignProperties(EntityPropertiesAssigner& assigner);
};

class EntityPropertiesAssigner {
public:
    EntityPropertiesAssigner() {
        numFloats = 0;
        numInts = 0;
        numBools = 0;
    }

    void SetInt(const std::string& property, int value);
    void SetFloat(const std::string& property, float value);
    void SetBool(const std::string& property, bool value);

// We want to be able to store this in Entities,
// which are in a union. So, we need static allocation
// on the strings.
private:
    char intStrings[8][32];
    int ints[8];
    int numInts;

    char floatStrings[8][32];
    float floats[8];
    int numFloats;

    char boolStrings[8][32];
    bool bools[8];
    int numBools;

    friend EntityProperties;
};
