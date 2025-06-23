#include "Medal.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "EntityList.h"
#include "Helpers/Shared_Ease.h"
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
    materials_[0].specularProperties.color = vec4(1.0f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 1.5f;
    overlapbox_.bottom = 1.5f;
    overlapbox_.radius = 1.5f;
    collected_ = false;
    collectTimer_ = 0;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_Interpolate, true);
}

void Medal::Start() {
    spawnTransform_ = transform_;
    player_ = (Player*)entities_->FindEntityByType(Player::TYPEID);
}

constexpr int TRACK_TIME = 20;
constexpr int START_TRACK_TIME = 0;
constexpr int END_TRACK_TIME = TRACK_TIME;

constexpr int FOLLOW_TIME = 60;
constexpr int START_FOLLOW_TIME = TRACK_TIME;
constexpr int END_FOLLOW_TIME = START_FOLLOW_TIME + FOLLOW_TIME;

constexpr int GATHER_TIME = 20;
constexpr int START_GATHER_TIME = END_FOLLOW_TIME;
constexpr int END_GATHER_TIME = START_GATHER_TIME + GATHER_TIME;

constexpr vec3 TRACK_SCALE = vec3(1.0f);

void Medal::Update() {
    if (!collected_)
        return;

    vec3 trackPos = player_->transform_.position;
    trackPos.y += 8.0f;

    if (collectTimer_ >= START_TRACK_TIME && collectTimer_ < END_TRACK_TIME) {
        float t = (float)(collectTimer_ - START_TRACK_TIME) / TRACK_TIME;
        transform_.position = mix(spawnTransform_.position, trackPos, EaseOutCubic(t));
        transform_.scale = mix(spawnTransform_.scale, TRACK_SCALE, EaseOutQuad(t));
    }

    if (collectTimer_ >= START_FOLLOW_TIME && collectTimer_ < END_FOLLOW_TIME) {
        transform_.position = trackPos;
        transform_.scale = TRACK_SCALE;
    }

    if (collectTimer_ >= START_GATHER_TIME && collectTimer_ < END_GATHER_TIME) {
        float t = (float)(collectTimer_ - START_GATHER_TIME) / GATHER_TIME;
        transform_.position = mix(trackPos, player_->transform_.position, EaseCubic(t));
        transform_.scale = mix(TRACK_SCALE, vec3(0.0f), EaseQuad(t));
    }

    if (collectTimer_ >= END_GATHER_TIME)
        destroy_ = true;

    collectTimer_++;
}

void Medal::OnDestroy() {
}

void Medal::OnOverlap(Entity* overlappedEntity) {
    if (collected_ || overlappedEntity != player_)
        return;

    collected_ = true;
}
