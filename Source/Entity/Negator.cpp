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
    negativeModifier_->sharpness = 0.25f;

    bubbleModifier_ = terrain_->CreateBubble();
    bubbleModifier_->position = transform_.position;
    bubbleModifier_->height = 0.0f;
    bubbleModifier_->inpower = 2.0f;
    bubbleModifier_->outpower = 8.0f;

    fader_ = DynamicFader();
    fader_.deactivateLength_ = 40.0f;
    fader_.activateLength_ = 40.0f;
}

void Negator::Start() {
    bubbleModifier_->radius = negativeModifier_->outerRadius;
    fader_.AddModifier(
        negativeModifier_, 
        &negativeModifier_->innerRadius, 
        negativeModifier_->outerRadius, 
        0.0f, 
        false, 
        EaseType::E_Quad,
        -negativeModifier_->outerRadius
    );
    fader_.AddModifier(bubbleModifier_, &bubbleModifier_->height, 0.0f, -200.0f, true);

    if (initialActive_)
        fader_.ActivateModifiers(true);
    else
        fader_.DeactivateModifiers(true);

    negativeModifier_->position = transform_.position;
    bubbleModifier_->position = transform_.position;
    bubbleModifier_->radius = negativeModifier_->outerRadius;
}

void Negator::Update() {
    negativeModifier_->position = transform_.position;
    bubbleModifier_->position = transform_.position;
    fader_.Update();
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
    fader_.Toggle();
}

bool Negator::GetIsTriggered() {
    return fader_.IsActive();
}
