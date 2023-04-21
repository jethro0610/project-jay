#pragma once
#include <json.hpp>
#include <algorithm>
#include "../../Helpers/LoadHelpers.h"
#include "../../Constants/EntityConstants.h"
#include "../../Types/EntityTypes.h"

const uint8_t MAX_COMPONENT_TYPES = 32;
struct Component {
    Component(){};

public:
    virtual void Load(nlohmann::json& data, EntityID entity) = 0;
};
