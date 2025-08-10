#include "ProjectileBell.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Bumper.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies ProjectileBell::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties ProjectileBell::GetStaticProperties() {
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

void ProjectileBell::Init(Entity::InitArgs args) {
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
    overlapbox_.bottom = 0.0f;
    overlapbox_.radius = 1.25f;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_Homeable, true);
    SetFlag(EF_ProjectileLockable, true);

    timer_ = 0;
}

void ProjectileBell::OnOverlap(Entity* overlappedEntity) {
    if (timer_ > 0)
        return;

    if (overlappedEntity->typeId_ == Player::TYPEID) {
        Player* player = (Player*)overlappedEntity;
        player->EndHoming();
    }
    if (overlappedEntity->typeId_ == Bumper::TYPEID) {
        Bumper* bumper = (Bumper*)overlappedEntity;
        bumper->StopTracking();
    }
    overlappedEntity->velocity_.y = 80.0f;
    overlappedEntity->hitlag_ = 4;
    hitlag_ = 4;
    stun_ = true;
    timer_ = 20;
}

void ProjectileBell::Update() {
    if (timer_ > 0)
        timer_--;
}
