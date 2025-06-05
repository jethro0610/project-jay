#include "HoleMarker.h"
#include "Resource/ResourceManager.h"
#include "Time/Time.h"
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

    materials_[0].lightVolumeProperties = LightVolumeProperties::Default();
    materials_[0].lightVolumeProperties.color = vec4(1.0f);
    materials_[0].lightVolumeProperties.stepDistance = 1.0f;
    materials_[0].lightVolumeProperties.accumulationStrength = 0.001f;

    materials_[0].lightVolumeProperties.origin = transform_.position;
    materials_[0].lightVolumeProperties.radius = transform_.scale.x;
    materials_[0].lightVolumeProperties.falloffHeight = transform_.scale.y;

    materials_[0].textures[0] = resourceManager.GetTexture("t_render_d");
    materials_[0].numTextures = 1;
    materials_[0].volumetric = true;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void HoleMarker::EditorUpdate() {
    materials_[0].lightVolumeProperties.origin = transform_.position;
    materials_[0].lightVolumeProperties.radius = transform_.scale.x;
    materials_[0].lightVolumeProperties.falloffHeight = transform_.scale.y;
}

void HoleMarker::RenderUpdate() {
    // materials_[0].lightVolumeProperties.color.r = sin(GlobalTime::GetTime() + 10.0f);
    // materials_[0].lightVolumeProperties.color.g = sin(GlobalTime::GetTime() + 20.0f);
    // materials_[0].lightVolumeProperties.color.b = sin(GlobalTime::GetTime() + 30.0f);
}
