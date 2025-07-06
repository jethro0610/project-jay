#include "PopInCrystalBreaker.h"
#include "Resource/ResourceManager.h"
#include "PopInCrystal.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies PopInCrystalBreaker::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties PopInCrystalBreaker::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
            {"p_targetid", &targetId_}
        },
        {
            // Bools
        }
    };
}

void PopInCrystalBreaker::Init(Entity::InitArgs args) {
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

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    SetFlag(EF_RecieveHits, true);
}

void PopInCrystalBreaker::Start() {
    target_ = (PopInCrystal*)entities_->FindEntityById(targetId_);
}

void PopInCrystalBreaker::OnHurt(HurtArgs args) {
    target_->Break();
    destroy_ = true;
}
