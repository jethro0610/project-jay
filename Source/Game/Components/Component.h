#pragma once
#include <json.hpp>
#include <algorithm>
#include "../../Helpers/LoadHelpers.h"
#include "../Entity/EntityLimits.h"
#include "../Entity/EntityID.h"

const int MAX_COMPONENT_TYPES = 32;
struct Component {
    Component(){};

public:
    virtual void Load(nlohmann::json& data, EntityID entity) = 0;
};
