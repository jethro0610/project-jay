#include "Negator.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
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
}

void Negator::Start() {
    negativeModifier_->active = initialActive_;
    negativeModifier_->position = transform_.position;
}

void Negator::Update() {
    negativeModifier_->position = transform_.position;
}

#ifdef _DEBUG
void Negator::EditorUpdate() {
    negativeModifier_->active = DBG_preview_;
    negativeModifier_->position = transform_.position;
}
#endif

void Negator::OnDestroy() {
    terrain_->FreeNegative(negativeModifier_);
}

void Negator::OnTrigger() {
    negativeModifier_->active = !negativeModifier_->active;
}

bool Negator::GetIsTriggered() {
    return negativeModifier_->active;
}
