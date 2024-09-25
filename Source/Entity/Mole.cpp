#include "Mole.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "EntityList.h"
#include "TimedHitbox.h"
#include "Helpers/Random.h"
using namespace glm;

static constexpr float SPEED = 60.0f;
static constexpr float FRICTION = 0.05f;
static constexpr float SPEED_DECAY = 1.0f - FRICTION;
static constexpr float ACCELERATION = ((SPEED / SPEED_DECAY) - SPEED);

EntityDependendies Mole::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void Mole::Init(Entity::InitArgs args) {
    SetFlag(EF_Targetable, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void Mole::Update() {
    timer_++;
    if (timer_ > 60) {
        vec2 terrainDistance;
        vec3 spawnPos;
        do {
            spawnPos = transform_.position + RandomVectorPlanar(100.0f);
            terrainDistance = terrain_->GetDistance(spawnPos);
            spawnPos.y = terrainDistance.y;
        }
        while(terrainDistance.x > -20.0f || entities_->IsAnyOverlapping(*this));

        TimedHitbox& entity = (TimedHitbox&)entities_->CreateEntity(TimedHitbox::TYPEID);
        entity.transform_.position = spawnPos;
        entity.lifespan_ = 10;
        entity.transform_.scale = vec3(20.0f, 50.0f, 20.0f);
        entity.spreadRadius_ = 1.0f;
        entity.hitbox_.knocback.x = 30.0f;
        entity.hitbox_.knocback.y = 30.0f;

        timer_ = 0;
    }
}
