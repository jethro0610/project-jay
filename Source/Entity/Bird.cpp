#include "Bird.h"
#include "Player.h"
#include "Resource/ResourceManager.h"
#include "Seed/SeedManager.h"
#include "Game/Clock.h"
#include "Game/GlobalNoise.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Helpers/Random.h"
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

EntityDependendies Bird::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void Bird::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 2.0f;
    overlapbox_.bottom = 2.0f;
    overlapbox_.radius = 2.0f;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_Homeable, true);
    SetFlag(EF_Interpolate, true);

    startPos_ = transform_.position;
    sinOffset_ = RandomVector(10000.0f);
}

void Bird::Update() {
    /*
    noisePosition_ += 0.25f; 
    float rotationSpeed = GlobalNoise::Sample(noisePosition_, noiseOffset_.y) * 0.15f;
    direction_ = rotate(direction_, rotationSpeed, Transform::worldUp);
    direction_ = normalize(direction_);
    transform_.position += direction_ * 0.75f;

    vec2 planarPos = vec2(transform_.position.x, transform_.position.z);
    float height = GlobalNoise::Sample(planarPos * 0.25f + noiseOffset_);
    height = (height + 1.0f) * 0.5f;
    height = lerp(20.0f, 50.0f, height);
    transform_.position.y = terrain_->GetRawHeight(vec2(transform_.position.x, transform_.position.z)) + height;
    */

    transform_.position.x = startPos_.x + sin(GlobalTime::GetTime() + sinOffset_.x) * 25.0f;
    transform_.position.z = startPos_.z + sin(GlobalTime::GetTime() + sinOffset_.z) * 25.0f;
    transform_.position.y = 
        terrain_->GetHeight(transform_.position) + 
        40.0f + 
        sin(GlobalTime::GetTime() + sinOffset_.y) * 20.0f +
        sin(GlobalTime::GetTime() * 5.0f + sinOffset_.y) * 2.0f;
}

constexpr float JUMP_STRENGTH = 150.0f;
constexpr float DAY_SECTION_PERCENT = 0.20f;
void Bird::OnOverlap(Entity* overlappedEntity) {
    // Temp
    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = JUMP_STRENGTH;

    if (overlappedEntity->typeId_ == Player::TYPEID)
        ((Player*)overlappedEntity)->EndHoming();

    seedManager_->CreateMultipleSeed(
        transform_.position, 
        Clock::TIME_IN_DAY_SECTION * DAY_SECTION_PERCENT, 
        20.0f, 
        overlappedEntity, 
        vec3(0.0f, 30.0f, 0.0f)
    );
    destroy_ = true;
}
