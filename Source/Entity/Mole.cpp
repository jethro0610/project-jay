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
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_SendPush, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    for (int i = 0; i < NUM_PILLARS; i++)
        pillars_[i] = nullptr;
}

void Mole::Update() {
    for (int i = 0; i < NUM_PILLARS; i++) {
        if (pillars_[i] == nullptr)
            continue;

        if (!pillars_[i]->alive_)
            pillars_[i] = nullptr;
    }

    for (int i = 0; i < NUM_PILLARS; i++) {
        if (pillars_[i] != nullptr)
            continue;

        vec3 spawnPos;
        vec3 scale = BASE_PILLAR_SCALE * RandomFloatRange(0.25f, 1.25f);
        do {
            spawnPos = transform_.position + RandomVectorPlanar(500.0f);
            float terrianHeight = terrain_->GetHeight(spawnPos);
            spawnPos.y = terrianHeight - scale.y;
        }
        while(!terrain_->PointIsInSameIsland(transform_.position, spawnPos) || !DistantFromPillars(spawnPos));

        Transform spawnTransform;
        spawnTransform.position = spawnPos;
        spawnTransform.scale = scale;
        RisePillar& entity = (RisePillar&)entities_->CreateEntity(RisePillar::TYPEID, spawnTransform);
        pillars_[i] = &entity;
    }
}

bool Mole::DistantFromPillars(vec3 pos) {
    bool distant = true;
    for (int i = 0; i < 128; i++) {
        if (!(*entities_)[i].alive_)
            continue;

        Entity& entity = (*entities_)[i];

        vec3 planarPillar = vec3(entity.transform_.position.x, 0.0f, entity.transform_.position.z);
        vec3 planarPos = vec3(pos.x, 0.0f, pos.z);
        if (distance(planarPillar, planarPos) < 50.0f) {
            distant = false;
            break;
        }
    }
    return distant;
}
