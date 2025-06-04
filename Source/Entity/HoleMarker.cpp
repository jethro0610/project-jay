#include "HoleMarker.h"
#include "Resource/ResourceManager.h"
using namespace glm;

EntityDependendies HoleMarker::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HoleMarker::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
        },
        {
            // Bools
        }
    };
}

void HoleMarker::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_hole");
    materials_[0].castShadows = false;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    materials_[0].triangleType = TriangleType::BACKFACE;
    materials_[0].transparencyType = TRANSPARENCY_ADDITIVE;
    materials_[0].raymarchVolume.origin = transform_.position;
    materials_[0].raymarchVolume.radius = transform_.scale.x;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

