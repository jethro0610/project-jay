#include "Mole.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "EntityList.h"
#include "RisePillar.h"
#include "Helpers/Random.h"
using namespace glm;

static constexpr float SPEED = 60.0f;
static constexpr float FRICTION = 0.05f;
static constexpr float SPEED_DECAY = 1.0f - FRICTION;
static constexpr float ACCELERATION = ((SPEED / SPEED_DECAY) - SPEED);
static constexpr vec3 BASE_PILLAR_SCALE = vec3(7.0f, 30.0f, 7.0f);

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

    for (int i = 0; i < 5; i++) {
        vec2 terrainDistance;
        vec3 spawnPos;
        vec3 scale = BASE_PILLAR_SCALE * RandomFloatRange(0.75f, 1.25f);
        do {
            spawnPos = transform_.position + RandomVectorPlanar(200.0f);
            terrainDistance = terrain_->GetDistance(spawnPos);
            spawnPos.y = terrainDistance.y - scale.y;
        }
        while(terrainDistance.x > -20.0f || entities_->IsAnyOverlapping(*this));

        Transform spawnTransform;
        spawnTransform.position = spawnPos;
        spawnTransform.scale = scale;
        RisePillar& entity = (RisePillar&)entities_->CreateEntity(RisePillar::TYPEID, spawnTransform);
    }
}

void Mole::Update() {
    return;
    timer_++;
    if (timer_ > 120) {
        vec2 terrainDistance;
        vec3 spawnPos;
        vec3 scale = BASE_PILLAR_SCALE * RandomFloatRange(0.75f, 1.25f);
        do {
            spawnPos = transform_.position + RandomVectorPlanar(200.0f);
            terrainDistance = terrain_->GetDistance(spawnPos);
            spawnPos.y = terrainDistance.y - scale.y;
        }
        while(terrainDistance.x > -20.0f || entities_->IsAnyOverlapping(*this));

        Transform spawnTransform;
        spawnTransform.position = spawnPos;
        spawnTransform.scale = scale;
        RisePillar& entity = (RisePillar&)entities_->CreateEntity(RisePillar::TYPEID, spawnTransform);

        timer_ = 0;
    }
}
