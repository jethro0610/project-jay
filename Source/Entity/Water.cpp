#include "Water.h"
#include "Resource/ResourceManager.h"
#include "Helpers/PhysicsHelpers.h"
#include "Terrain/Terrain.h"
#include "Game/WaterManager.h"
using namespace glm;

EntityDependendies Water::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Water::GetStaticProperties() {
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

void Water::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.0f;

    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Overlap, true);
    SetFlag(EF_Interpolate, true);
}

constexpr float GRAVITY = 6.0f;
void Water::Update() {
    velocity_.y -= GRAVITY;     
    if (transform_.position.y < terrain_->GetHeight(transform_.position))
        destroy_ = true;
}

void Water::Shoot(const vec3& target, float yVel) {
    velocity_ = GetProjectileVelocityToPoint(transform_.position, target, GRAVITY, yVel);
}

void Water::OnOverlap(Entity* overlappedEntity) {
    if (velocity_.y < 0.0f)
        destroy_ = true;
}

void Water::OnDestroy() {
    waterManager_->Wet(transform_.position, 50.0f);
}
