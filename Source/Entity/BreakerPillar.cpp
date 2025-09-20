#include "BreakerPillar.h"
#include "Resource/ResourceManager.h"
using namespace glm;

EntityDependendies BreakerPillar::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties BreakerPillar::GetStaticProperties() {
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

void BreakerPillar::Init(Entity::InitArgs args) {
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

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.25f;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_SendPush, true);
}

void BreakerPillar::OnOverlap(Entity* overlappedEntity) {
    float velLength = length2(overlappedEntity->velocity_);
    if (velLength < 80.0f * 80.0f)
        return;

    overlappedEntity->hitlag_ = 8;
    hitlag_ = 8;
    stun_ = true;
    destroy_ = true;
}
