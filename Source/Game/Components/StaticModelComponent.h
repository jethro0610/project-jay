#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <string>

const uint8_t MAX_MESHES_PER_MODEL = 8;

struct StaticModelCProperties {
    std::string& model;
    std::string* materials;
};

struct StaticModelComponent : public Component {
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

    std::string GetName() const { return "static_model"; }
    void Load(nlohmann::json& data, uint16_t entity) {
        model[entity] = data["model"].get<std::string>();
        materials[entity][0] = data["material0"].get<std::string>();
    }
};
