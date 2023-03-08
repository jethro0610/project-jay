#pragma once
#include <string>
#include <tuple>
#include <map>

struct ComponentProperty {
    std::string name;
    std::string type;
};

struct ComponentDesc {
    std::string name;
    uint8_t propertyCount;
    ComponentProperty properties[8];
};
