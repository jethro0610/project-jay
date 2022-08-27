#pragma once
#include "../Entity/Entity.h"
#include <string>

struct StaticModelCProperties {
    std::string& model;
};

struct StaticModelComponent {
    std::string model[MAX_ENTITIES];

    StaticModelComponent() {
        std::fill_n(model, MAX_ENTITIES, "");
    };
    StaticModelComponent(const StaticModelComponent&) = delete;
    StaticModelComponent& operator=(const StaticModelComponent&) = delete;

    StaticModelCProperties operator[](int index) {
        return StaticModelCProperties {
            model[index]
        };
    }

    static int ID;
};