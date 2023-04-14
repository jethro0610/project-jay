#pragma once
#include <json.hpp>
#include <algorithm>
#include "../../Helpers/LoadHelpers.h"
#include "../../Constants/EntityConstants.h"

const uint8_t MAX_COMPONENT_TYPES = 32;
struct Component {
    Component(){};
    public:
        uint8_t id;
        virtual std::string GetName() const = 0;
        virtual void Load(nlohmann::json& data, EntityID entity) = 0;
};
