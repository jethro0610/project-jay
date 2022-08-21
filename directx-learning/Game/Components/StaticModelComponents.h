#pragma once
#include "EntityConstants.h"
#include <string>

#define NO_MODEL ""

struct StaticModelComponents {
    std::string model[MAX_ENTITIES];

    StaticModelComponents() {
        std::fill_n(model, MAX_ENTITIES, NO_MODEL);
    };
    StaticModelComponents(const StaticModelComponents&) = delete;
    StaticModelComponents& operator=(const StaticModelComponents&) = delete;
};