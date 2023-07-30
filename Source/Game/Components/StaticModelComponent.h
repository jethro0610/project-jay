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

    Model model[MAX_ENTITIES];
    Material materials[MAX_ENTITIES][MAX_MESHES_PER_MODEL];

    StaticModelComponent() {

    };

    StaticModelComponent(const StaticModelComponent&) = delete;
    StaticModelComponent& operator=(const StaticModelComponent&) = delete;

    static constexpr std::string GetName() { return "static_model"; }
    static constexpr int GetID() { return 9; }

    void Load(nlohmann::json& data, EntityID entity) {
        ASSERT((renderer != nullptr), "Static Model Component has no access to renderer");
        model[entity] = renderer->GetModel(data["model"].get<std::string>());

        auto materialData = data["materials"];
        ASSERT((materialData.size() <= MAX_MESHES_PER_MODEL), "Too many materials on entity description");
        for (int i = 0; i < materialData.size(); i++)
            materials[entity][i] = renderer->GetMaterial(materialData[i]);
    }
};
