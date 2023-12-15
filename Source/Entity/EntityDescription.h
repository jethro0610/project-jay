#pragma once
#include "Debug/DebugName.h"
#include <nlohmann/json.hpp>
#include <string>
struct EntityDescription {
    USE_DEBUG_NAME;
    nlohmann::json data;
};
