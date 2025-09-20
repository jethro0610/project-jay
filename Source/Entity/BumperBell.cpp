#include "BumperBell.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
using namespace glm;

EntityDependendies BumperBell::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties BumperBell::GetStaticProperties() {
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

void BumperBell::Init(Entity::InitArgs args) {
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
    overlapbox_.bottom = -0.95f;
    overlapbox_.radius = 1.25f;
    SetFlag(EF_ProjectileLockable, true);
    SetFlag(EF_Overlap, true);

    timer_ = 0;
    broken_ = false;
}

void BumperBell::Update() {
    if (timer_ > 0)
        timer_--;
}

void BumperBell::OnOverlap(Entity* overlappedEntity) {
    if (timer_ > 0)
        return;

    timer_ = 60;
    overlappedEntity->hitlag_ = 8;
    hitlag_ = 8;
    overlappedEntity->stun_ = true;
    // destroy_ = true;

    if (!broken_ && overlappedEntity->GetFlag(EF_SeedReleaser))
        seedManager_->CreateMultipleSeed(GetTargetPoint(), 500, 30.0f, overlappedEntity);

    broken_ = true;
}

vec3 BumperBell::GetTargetPoint() {
    vec3 pos = transform_.position;
    pos.y += 1.0f * transform_.scale.y;
    return pos;
}
