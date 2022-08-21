#pragma once
#pragma once
#include "EntityConstants.h"

struct ActiveEntityComponents {
    bool active[MAX_ENTITIES];

    ActiveEntityComponents() {
        std::fill_n(active, MAX_ENTITIES, false);
    };
    ActiveEntityComponents(const ActiveEntityComponents&) = delete;
    ActiveEntityComponents& operator=(const ActiveEntityComponents&) = delete;
};