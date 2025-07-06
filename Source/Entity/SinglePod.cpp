#include "SinglePod.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies SinglePod::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties SinglePod::GetStaticProperties() {
    return {
        {
        },
        {
            {"p_seeds", &seeds_},
        },
        {}
    };
}

void SinglePod::Init(Entity::InitArgs args) {
    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_Interpolate, true);
    traceDistance_ = 1000.0f;

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 2.0f;
    seeds_ = 100;
}

void SinglePod::OnHurt(HurtArgs args) {
    SetFlag(EF_RecieveHits, false);
    seedManager_->CreateMultipleSeed(transform_.position, seeds_, 20.0f, args.attacker);
    destroy_ = true;
}

int SinglePod::GetSeeds() {
    return seeds_;
}
