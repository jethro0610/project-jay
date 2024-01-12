#include "TrailFish.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Helpers/Random.h"
#include "Terrain/Terrain.h"
using namespace glm;

EntityDependendies TrailFish::GetDeps() {
    return {
        "st_tpillar"
    };
}

void TrailFish::Init(Entity::InitArgs args) {
    Entity::Init(args);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].selectedShader = resourceManager.GetShader("vs_static", "fs_selected");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f);
    materials_[0].properties.fresnelBrightness = 0.15f;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    desiredMovement_ = transform_.GetForwardVector();
}

void TrailFish::Update() {
    float curDistance = terrain_->GetDistance(transform_.position).x;
    if (curDistance > -50.0f) {
        vec3 centerOffset = RandomVectorPlanar(100.0f);
        desiredMovement_ = normalize(centerOffset - vec3(transform_.position.x, 0.0f, transform_.position.z));
    }

    velocity_.y -= 1.0f;
    velocity_.x += desiredMovement_.x * ACCELERATION;
    velocity_.z += desiredMovement_.z * ACCELERATION;
    velocity_.x *= SPEED_DECAY;
    velocity_.z *= SPEED_DECAY;
    if (length(desiredMovement_) > 0.001f) {
        quat rotation = quatLookAtRH(normalize(desiredMovement_), Transform::worldUp);
        transform_.rotation = slerp(transform_.rotation, rotation, ROTATION_SPEED);
    }
}
