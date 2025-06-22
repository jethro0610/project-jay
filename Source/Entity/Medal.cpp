#include "Medal.h"
#include "Resource/ResourceManager.h"
#include "Game/ScoreKeeper.h"
#include "Player.h"
using namespace glm;

EntityDependendies Medal::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Medal::GetStaticProperties() {
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

void Medal::Init(Entity::InitArgs args) {
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

    overlapbox_.top = 1.5f;
    overlapbox_.bottom = 1.5f;
    overlapbox_.radius = 1.5f;

    SetFlag(EF_Overlap, true);
}

void Medal::OnDestroy() {
    scoreKeeper_->numMedals_++;
}

void Medal::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    destroy_ = true;
}
