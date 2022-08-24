#pragma once
#include "EntityConstants.h"
#include <string>

struct StaticModelCProperties {
    std::string& model;
};

struct StaticModelComponents {
    std::string model[MAX_ENTITIES];

    StaticModelComponents() {
        std::fill_n(model, MAX_ENTITIES, "");
    };
    StaticModelComponents(const StaticModelComponents&) = delete;
    StaticModelComponents& operator=(const StaticModelComponents&) = delete;

    StaticModelCProperties operator[](int index) {
        return StaticModelCProperties {
            model[index]
        };
    }

    static int ID;
};