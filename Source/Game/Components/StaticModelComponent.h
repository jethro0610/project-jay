#pragma once
#include <string>
#include "Component.h"

const uint8_t MAX_MESHES_PER_MODEL = 8;

struct StaticModelComponent : public Component {
    std::string model[MAX_ENTITIES];
    std::string materials[MAX_ENTITIES][MAX_MESHES_PER_MODEL];

    StaticModelComponent() {
        std::fill_n(model, MAX_ENTITIES, "NONE_MODEL");
        std::fill_n(&materials[0][0], MAX_ENTITIES * MAX_MESHES_PER_MODEL, "NONE_MATERIAL");
    };
    StaticModelComponent(const StaticModelComponent&) = delete;
    StaticModelComponent& operator=(const StaticModelComponent&) = delete;

    std::string GetName() const { return "static_model"; }
    void Load(nlohmann::json& data, EntityID entity) {
        model[entity] = data["model"].get<std::string>();
        materials[entity][0] = data["material0"].get<std::string>();
    }
};
