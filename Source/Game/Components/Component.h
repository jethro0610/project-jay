#pragma once
#include <json.hpp>

struct Component {
    Component(){};
    public:
        uint8_t id;
        virtual std::string GetName() const = 0;
        virtual void Load(nlohmann::json& data, uint16_t entity) = 0;
};
