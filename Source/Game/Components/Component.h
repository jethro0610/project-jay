#pragma once
#include <json.hpp>

struct Component {
    Component(){};
    public:
        virtual void Load(nlohmann::json& data, uint16_t entity) = 0;
};
