#include "Negator.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"
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
            {"p_radius", &DYN_MOD_RADIUS(*negativeModifier_) }
        },
        {
            // Ints
        },
        {
            // Bools
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
    negativeModifier_ = terrain_->CreateNegative();
    DYN_MOD_POS_X(*negativeModifier_) = transform_.position.x;
    DYN_MOD_POS_Y(*negativeModifier_) = transform_.position.z;
    DYN_MOD_SET_ACTIVE(*negativeModifier_, false);
}

void Negator::Start() {
    DYN_MOD_SET_ACTIVE(*negativeModifier_, true);
    DYN_MOD_POS_X(*negativeModifier_) = transform_.position.x;
    DYN_MOD_POS_Y(*negativeModifier_) = transform_.position.z;
}

void Negator::Update() {
    DYN_MOD_RADIUS(*negativeModifier_) = 100.0f + sin(GlobalTime::GetTime()) * 20.0f;
}

void Negator::OnDestroy() {
    terrain_->FreeNegative(negativeModifier_);
}

void Negator::EditorUpdate() {
    DYN_MOD_SET_ACTIVE(*negativeModifier_, true);
    DYN_MOD_VALUE(*negativeModifier_) = 20.0f;
    DYN_MOD_POS_X(*negativeModifier_) = transform_.position.x;
    DYN_MOD_POS_Y(*negativeModifier_) = transform_.position.z;
}
