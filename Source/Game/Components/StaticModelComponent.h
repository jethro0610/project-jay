#pragma once
#include "../Entity/Entity.h"
#include <string>

#include "ComponentDesc.h"

// TODO: Move this to raw model or something else
const uint8_t MAX_MESHES_PER_MODEL = 4;

struct StaticModelCProperties {
    std::string& model;
    std::string* materials;
};

struct StaticModelComponent {
    std::string model[MAX_ENTITIES];
    std::string materials[MAX_ENTITIES][MAX_MESHES_PER_MODEL];

    StaticModelComponent() {
        std::fill_n(model, MAX_ENTITIES, "NONE_MODEL");
        std::fill_n(&materials[0][0], MAX_ENTITIES * MAX_MESHES_PER_MODEL, "NONE_MATERIAL");
    };
    StaticModelComponent(const StaticModelComponent&) = delete;
    StaticModelComponent& operator=(const StaticModelComponent&) = delete;

    StaticModelCProperties operator[](int index) {
        return StaticModelCProperties {
            model[index],
            &materials[index][0]
        };
    }

    inline static int ID = 11;
    inline static ComponentDesc description {
        "Static Model",
        2,
        {
            {"Model", "string"},
            {"Material 1", "string"},
            {"Material 2", "string"},
            {"Material 3", "string"},
            {"Material 4", "string"},
        }
    };
};
