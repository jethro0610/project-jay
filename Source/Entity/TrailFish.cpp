#include "TrailFish.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Helpers/Random.h"
#include "Terrain/Terrain.h"
#include "Bomb.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies TrailFish::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void TrailFish::Init(Entity::InitArgs args) {
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);

    spawnTimer_ = 0;
    desiredMovement_ = vec3(0.0f);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    desiredMovement_ = transform_.GetForwardVector();
}

void TrailFish::Update() {
    float curDistance = terrain_->GetDistance(transform_.position).x;
    float forwardDistance = terrain_->GetDistance(transform_.position + desiredMovement_ * 10.0f, TA_Low).x;
    if (curDistance > -50.0f && forwardDistance - curDistance > 0.0f) {
        vec3 centerOffset = RandomVectorPlanar(200.0f);
        desiredMovement_ = normalize(centerOffset - vec3(transform_.position.x, 0.0f, transform_.position.z));
        // desiredMovement_ = rotate(-desiredMovement_, RandomFloatRange(-25.0f, 25.0f), Transform::worldUp);
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

    spawnTimer_++;
    if (spawnTimer_ > SPAWN_INTERVAL && !entities_->IsAnyOverlapping(*this)) {
        Entity& bomb = entities_->CreateEntity(Bomb::TYPEID, transform_);
        spawnTimer_ = 0;
    }
}
