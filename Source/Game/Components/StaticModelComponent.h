#pragma once
#include <bitset>
#include <string>
#include "Component.h"
#include "../../Helpers/Assert.h"
#include "../../Rendering/Material.h"
#include "../../Rendering/Model.h"
#include "../ResourceManager.h"

class StaticModelComponent : public Component {
public:
    // Kind of a hack, but we need to assign this renderer before
    // loading any models
    ResourceManager* resourceManager;

    std::array<Model*, MAX_ENTITIES> model;
    std::array<std::vector<Material*>, MAX_ENTITIES> materials;

    StaticModelComponent() {
        model.fill(nullptr);
    };

    StaticModelComponent(const StaticModelComponent&) = delete;
    StaticModelComponent& operator=(const StaticModelComponent&) = delete;

    static constexpr std::string GetName() { return "static_model"; }
    static constexpr int GetID() { return 9; }

    void Load(nlohmann::json& data, EntityID entity) {
        materials[entity].clear();
        materials[entity].shrink_to_fit();

        ASSERT((resourceManager != nullptr), "Static Model Component has no access to renderer");
        std::string name = GetString(data, "model", "null_model");
        model[entity] = resourceManager->GetModel(name);

        auto& materialData = data["materials"];
        for (auto& materialName : materialData)
            materials[entity].push_back(resourceManager->GetMaterial(materialName));
    }
};
