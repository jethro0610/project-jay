#include "MeteorHole.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Game/Clock.h"
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
            {"p_radius", &radius_},
            {"p_bubbleradius", &bubbleRadius_},
            {"p_k", &negative_->sharpness},
            {"p_height", &height_}
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

    radius_ = 100.0f;
    bubbleRadius_ = 50.0f;
    height_ = 100.0f;

    negative_ = terrain_->CreateNegative();
    negative_->active = false;
    negative_->sharpness = 0.5f;

    bubble_ = terrain_->CreateBubble();
    bubble_->active = false;
    bubble_->inpower = 2.0f;
    bubble_->outpower = 2.0f;
    fader_ = DynamicFader();
    fader_.activateLength_ = 60.0f * 0.25f;
    fader_.deactivateLength_ = 60.0f * 0.25f;
}

void MeteorHole::Start() {
    negative_->innerRadius = -radius_;
    fader_.AddModifier(negative_, &negative_->outerRadius, 0.0f, radius_, false, E_Elastic);
    fader_.AddModifier(bubble_, &bubble_->radius, 0.0f, radius_ + bubbleRadius_, false, E_Elastic);
    fader_.AddModifier(bubble_, &bubble_->height, 0.0f, height_, false, E_Elastic);
    fader_.DeactivateModifiers(true);
    initClock_ = clock_->time_;
}

void MeteorHole::OnDestroy() {
    terrain_->FreeNegative(negative_);
    terrain_->FreeBubble(bubble_);
}

void MeteorHole::PreUpdate() {
    if (clock_->time_ != initClock_)
        Boom();

    negative_->position = transform_.position;
    bubble_->position = transform_.position;
    fader_.Update();
}

void MeteorHole::EditorUpdate() {
    negative_->outerRadius = radius_;
    negative_->innerRadius = -radius_;
    bubble_->height = height_;
    bubble_->radius = radius_ + bubbleRadius_;

    if (DBG_preview_) {
        negative_->active = true;
        bubble_->active = true;
    }
    else {
        negative_->active = false;
        bubble_->active = false;
    }

    negative_->position = transform_.position;
    bubble_->position = transform_.position;
    fader_.Update();
}

void MeteorHole::Boom() {
    fader_.StartActivate();
}
