#include "MeteorHole.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
using namespace glm;

EntityDependendies MeteorHole::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties MeteorHole::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_radius", &negative_->outerRadius},
            {"p_k", &negative_->sharpness},
            {"p_height", &bubble_->height}
        },
        {
            // Ints
        },
        {
            // Bools
        }
    };
}

void MeteorHole::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    negative_ = terrain_->CreateNegative();
    negative_->active = false;

    bubble_ = terrain_->CreateBubble();
    bubble_->active = false;
}

void MeteorHole::OnDestroy() {
    terrain_->FreeNegative(negative_);
    terrain_->FreeBubble(bubble_);
}

void MeteorHole::PreUpdate() {
    negative_->position = transform_.position;
    bubble_->position = transform_.position;
}

void MeteorHole::EditorUpdate() {
    negative_->active = DBG_preview_;
    negative_->position = transform_.position;

    bubble_->active = DBG_preview_;
    bubble_->position = transform_.position;
}
