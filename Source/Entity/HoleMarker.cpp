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
    materials_[0].triangleType = TriangleType::BACKFACE;
    materials_[0].transparencyType = TRANSPARENCY_ADDITIVE_BRIGHT;
    materials_[0].raymarchVolume.origin = transform_.position;
    materials_[0].raymarchVolume.radius = transform_.scale.x;
    materials_[0].textures[0] = resourceManager.GetTexture("t_render_d");
    materials_[0].numTextures = 1;
    materials_[0].volumetric = true;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

