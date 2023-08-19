#pragma once
#include <string>
#include "Component.h"
#include "../../Helpers/Assert.h"
#include "../../Rendering/Material.h"
#include "../../Rendering/Model.h"
#include "../../Rendering/Renderer.h"

class StaticModelComponent : public Component {
public:
    // Kind of a hack, but we need to assign this renderer before
    // loading any models
    Renderer* renderer;

    std::array<Model, MAX_ENTITIES> model;
    std::array<std::vector<Material>, MAX_ENTITIES> materials;
    std::array<Skeleton, MAX_ENTITIES> skeleton;

    StaticModelComponent() {

    };

    StaticModelComponent(const StaticModelComponent&) = delete;
    StaticModelComponent& operator=(const StaticModelComponent&) = delete;

    static constexpr std::string GetName() { return "static_model"; }
    static constexpr int GetID() { return 9; }

    void Load(nlohmann::json& data, EntityID entity) {
        materials[entity].clear();
        materials[entity].shrink_to_fit();

        ASSERT((renderer != nullptr), "Static Model Component has no access to renderer");
        std::string name = GetString(data, "model", "null_model");
        model[entity] = renderer->GetModel(name);

        auto materialData = data["materials"];
        for (auto& materialName : materialData)
            materials[entity].push_back(renderer->GetMaterial(materialName));

        if (GetBoolean(data, "skeletal") == true)
            skeleton[entity] = renderer->GetSkeleton(name);
    }
};
