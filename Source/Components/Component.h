#pragma once
#include "Helpers/LoadHelpers.h"
#include "Entity/EntityLimits.h"
#include "Entity/EntityID.h"
#include <nlohmann/json.hpp>
#include <algorithm>

const int MAX_COMPONENT_TYPES = 32;
struct Component {
    Component(){};

public:
    virtual void Load(nlohmann::json& data, EntityID entity) = 0;
};
