#include "ItemToken.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"
#include "Player.h"
#include "Helpers/Shared_Ease.h"
#include "Helpers/Random.h"
using namespace glm;

EntityDependendies ItemToken::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void ItemToken::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    SetFlag(EF_Interpolate, true);
    player_ = nullptr;
    duration_ = 0.0f;
    lifetime_ = 0.0f;
    basePosition_ = transform_.position;
}

void ItemToken::Update() {
    if (player_ == nullptr)
        return;

    basePosition_ += spokeVelocity_ * GlobalTime::TIMESTEP;

    lifetime_ += GlobalTime::TIMESTEP;
    float t = lifetime_ / duration_;
    float ease = EaseQuad(t);
    transform_.position = mix(basePosition_, player_->transform_.position, ease);

    float height = terrain_->GetHeight(transform_.position);
    if (transform_.position.y < height)
        transform_.position.y = height;

    if (lifetime_ >= duration_) {
        player_->AddItem(item_, 1);
        destroy_ = true;
    }
}

void ItemToken::SetPlayerAndItem(Player* player, Item item, float durationOffset) {
    vec3 playerForward = player->transform_.GetForwardVector();
    player_ = player;
    item_ = item;
    duration_ = 0.75f + durationOffset;

    vec3 randomPlanar = RandomVectorPlanar();
    spokeVelocity_ = Transform::worldUp * 50.0f + playerForward * 75.0f + randomPlanar * 35.0f;
}
