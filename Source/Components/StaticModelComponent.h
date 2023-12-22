#pragma once
#include "Component.h"
#include "Resource/ResourceManager.h"
#include "Helpers/Assert.h"
#include <string>

class StaticModelComponent : public Component {
public:
    // Kind of a hack, but we need to assign this renderer before
    // loading any models
    ResourceManager* resourceManager;

    std::array<Model*, MAX_ENTITIES> model;
    std::array<std::vector<Material*>, MAX_ENTITIES> materials;
    #ifdef _DEBUG
    std::array<std::vector<Material*>, MAX_ENTITIES> selectedMaterials;
    #endif

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

        #ifdef _DEBUG
        selectedMaterials[entity].clear();
        selectedMaterials[entity].shrink_to_fit();
        #endif

        ASSERT((resourceManager != nullptr), "Static Model Component has no access to renderer");
        std::string name = GetString(data, "model", "null_model");
        model[entity] = resourceManager->GetModel(name);

        auto& materialData = data["materials"];
        for (auto& materialName : materialData) {
            materials[entity].push_back(resourceManager->GetMaterial(materialName));
            #ifdef _DEBUG
            selectedMaterials[entity].push_back(resourceManager->GetMaterial(materialName.get<std::string>() + "_selected"));
            #endif
        }
    }
};
