#pragma once
#include <json.hpp>

struct Component {
    Component(){};
    public:
        virtual uint8_t GetID() const = 0;
        virtual void Load(nlohmann::json& data, uint16_t entity) = 0;
};
