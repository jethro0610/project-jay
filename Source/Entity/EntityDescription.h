#pragma once
#include <nlohmann/json.hpp>
#include <string>
struct EntityDescription {
    #ifdef _DEBUG
    std::string DBG_name;
    #endif
    nlohmann::json data;
};
