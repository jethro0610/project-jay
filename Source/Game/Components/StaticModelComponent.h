#pragma once
#include "../Entity/Entity.h"
#include <string>

#define MAX_MESHES_PER_MODEL 8

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

    static int ID;
};