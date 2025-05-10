#include "Negator.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Helpers/Shared_Ease.h"
using namespace glm;

EntityDependendies Negator::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Negator::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_radius", &negativeModifier_->outerRadius },
            {"p_k", &negativeModifier_->sharpness }
        },
        {
            // Ints
        },
        {
            // Bools
            {"p_active", &initialActive_}
        }
    };
}

void Negator::Init(Entity::InitArgs args) {
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

    initialActive_ = false;

    negativeModifier_ = terrain_->CreateNegative();
    negativeModifier_->position = transform_.position;
    negativeModifier_->outerRadius = 100.0f;
    negativeModifier_->sharpness = 1.0f;
    negativeModifier_->innerRadius = 0.0f;
    deactivate_ = false;
    deactivateTimer_ = 0;

    bubbleModifier_ = terrain_->CreateBubble();
    bubbleModifier_->position = transform_.position;
    bubbleModifier_->height = 0.0f;
    bubbleModifier_->inpower = 0.5f;
    bubbleModifier_->outpower = 50.0f;
    bubbleModifier_->radius = negativeModifier_->outerRadius;
    negativeModifier_->sharpness = 1.0f;
    negativeModifier_->innerRadius = 0.0f;
    deactivate_ = false;
    deactivateTimer_ = 0;
}

void Negator::Start() {
    negativeModifier_->active = initialActive_;
    negativeModifier_->position = transform_.position;

    bubbleModifier_->active = initialActive_;
    bubbleModifier_->position = transform_.position;
    bubbleModifier_->radius = negativeModifier_->outerRadius;
}

void Negator::Update() {
    negativeModifier_->position = transform_.position;
    bubbleModifier_->position = transform_.position;

    if (deactivate_)
        deactivateTimer_++;

    if (deactivateTimer_ > 60) {
        negativeModifier_->active = false;
        bubbleModifier_->active = false;
        deactivate_ = false;
    }

    if (!deactivate_) {
        negativeModifier_->innerRadius = -negativeModifier_->outerRadius;
        bubbleModifier_->height = 0.0f;
    }
    else {
        float t = deactivateTimer_ / 60.0f;
        t = EaseOutCubic(t);
        negativeModifier_->innerRadius = mix(0.0f, negativeModifier_->outerRadius - 50.0f, t);
        bubbleModifier_->height = mix(-200.0f, 0.0f, t);
    }
}

#ifdef _DEBUG
void Negator::EditorUpdate() {
    negativeModifier_->active = DBG_preview_;
    negativeModifier_->position = transform_.position;

    bubbleModifier_->active = DBG_preview_;
    bubbleModifier_->position = transform_.position;
}
#endif

void Negator::OnDestroy() {
    terrain_->FreeNegative(negativeModifier_);
    terrain_->FreeBubble(bubbleModifier_);
}

void Negator::OnTrigger() {
    if (negativeModifier_->active) {
        deactivate_ = true;
        deactivateTimer_ = 0;
    }
    else {
        negativeModifier_->active = false;
        bubbleModifier_->active = false;
        deactivate_ = false;
    }
}

bool Negator::GetIsTriggered() {
    return negativeModifier_->active && !deactivate_;
}
