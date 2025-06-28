#include "HoleMarker.h"
#include "Resource/ResourceManager.h"
#include "Time/Time.h"
#include "Seed/SeedManager.h"
#include "Entity/EntityList.h"
#include "Player.h"
using namespace glm;

EntityDependendies HoleMarker::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HoleMarker::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
            {"p_seedsentity", &seedsPerEntity_},
            {"p_seedsend", &seedsEnd_},
            {"p_entities", &numEntities_},
        },
        {
            // Bools
        }
    };
}

void HoleMarker::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_hole");
    materials_[0].castShadows = false;
    materials_[0].triangleType = TriangleType::BACKFACE;
    // materials_[0].transparencyType = TRANSPARENCY_ADDITIVE;

    materials_[0].lightVolumeProperties = LightVolumeProperties::Default();
    materials_[0].lightVolumeProperties.color = vec4(1.0f, 0.95f, 0.75f, 1.0f);
    materials_[0].lightVolumeProperties.stepDistance = 4.0f;
    materials_[0].lightVolumeProperties.accumulationStrength = 0.0005f;

    materials_[0].lightVolumeProperties.origin = transform_.position;
    materials_[0].lightVolumeProperties.radius = transform_.scale.x;
    materials_[0].lightVolumeProperties.falloffHeight = transform_.scale.y;

    materials_[0].textures[0] = resourceManager.GetTexture("t_render_d");
    materials_[0].numTextures = 1;
    materials_[0].volumetric = true;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.0f;

    seedsPerEntity_ = 250;
    seedsEnd_ = 250;
    numEntities_ = 1;
    entityCount_ = 0;

    player_ = nullptr;
}

void HoleMarker::Start() {
    player_ = entities_->FindEntityByType(Player::TYPEID);
}

void HoleMarker::EditorUpdate() {
    materials_[0].lightVolumeProperties.origin = transform_.position;
    materials_[0].lightVolumeProperties.radius = transform_.scale.x;
    materials_[0].lightVolumeProperties.falloffHeight = transform_.scale.y;
}

void HoleMarker::RenderUpdate() {
    materials_[0].lightVolumeProperties.origin = transform_.position;
    materials_[0].lightVolumeProperties.radius = transform_.scale.x;
    materials_[0].lightVolumeProperties.falloffHeight = transform_.scale.y;

    if (explodeTime_ > 0.0f)
        explodeTime_ -= GlobalTime::GetDeltaTime();
    else if (explodeTime_ < 0.0f)
        explodeTime_ = 0.0f;

    materials_[0].lightVolumeProperties.accumulationStrength = 0.0005f + 0.0005f * (explodeTime_ / 0.35f);
}

void HoleMarker::EntityFellInHole(Entity* entity) {
    seedManager_->CreateMultipleSeed(entity->transform_.position, seedsPerEntity_, 20.0f, player_, vec3(0.0f, 50.0f, 0.0f));
    explodeTime_ = 0.35f;
    entityCount_++;
    if (entityCount_ >= numEntities_) {
        seedManager_->CreateMultipleSeed(entity->transform_.position, seedsEnd_, 20.0f, player_, vec3(0.0f, 50.0f, 0.0f));
    }
}

int HoleMarker::GetSeeds() {
    return seedsPerEntity_ * numEntities_ + seedsEnd_;
}
