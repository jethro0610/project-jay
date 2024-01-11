#include "TeleportPod.h"
#include "Seed/SeedManager.h"
#include "Terrain/Terrain.h"
#include "Resource/ResourceManager.h"
#include "Helpers/Random.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies TeleportPod::GetDeps() {
    return {
        "st_tpillar"
    };
}

void TeleportPod::Init(Entity::InitArgs args) {
    Entity::Init(args);

    shouldTeleport_ = false;
    teleportScaleTicks_ = 0.0f;
    scaleBeforeTeleport_ = vec3(0.0f);

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_SendPush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].selectedShader = resourceManager.GetShader("vs_static", "fs_selected");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void TeleportPod::Update() {
    if (hitlag_ == 0 && shouldTeleport_) {
        vec3 pos;
        vec2 terrainDistance;
        do {
            pos = RandomVectorPlanar(100.0f);
            terrainDistance = terrain_->GetDistance(pos);
            pos.y = terrainDistance.y;
        }
        while(terrainDistance.x > -20.0f);

        transform_.position = pos;
        scaleBeforeTeleport_ = transform_.scale;
        transform_.scale = vec3(0.0f);
        shouldTeleport_ = false;
        teleportScaleTicks_ = MAX_TELEPORT_SCALE_TICKS;
    }

    if (teleportScaleTicks_ > 0) {
        SetFlag(EF_RecieveHits, false);
        teleportScaleTicks_--;
        transform_.scale = mix(vec3(0.0f), scaleBeforeTeleport_, 1.0f - (float)teleportScaleTicks_ / MAX_TELEPORT_SCALE_TICKS);
    }
    else {
        SetFlag(EF_RecieveHits, true);
    }
}

void TeleportPod::OnHurt() {
    seedManager_->CreateMultipleSeed(transform_.position, 200, 20.0f);
    shouldTeleport_ = true;
}
