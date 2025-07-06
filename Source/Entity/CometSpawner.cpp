#include "CometSpawner.h"
#include "Comet.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Helpers/Random.h"
#include "Terrain/Terrain.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies CometSpawner::GetStaticDependencies() {
    return {
        "st_comet"
    };
}

EntityProperties CometSpawner::GetStaticProperties() {
    return {
        {
            {"p_radius", &spawnRadius_}
        },
        {
            {"p_seeds", &seeds_},
            {"p_interval", &interval_},
            {"p_simulcomets", &simulComets_},
            {"p_comets", &maxComets_}
        },
        {}
    };
}


void CometSpawner::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_default");

    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].castShadows = false;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    materials_[0].numTextures = 0;

    timer_ = 0;
    seeds_ = 100;
    spawnRadius_ = 0.0f;
    interval_ = 60;
    simulComets_ = 3;
    numComets_ = 0;
    maxComets_ = 6;
    totalComets_ = 0;
    doTimer_ = true;

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void CometSpawner::Update() {
    if (!doTimer_)
        return;

    timer_++;
    if (timer_ >= interval_) {
        timer_ = 0;

        Transform cometTransform;

        vec2 pos;
        vec2 terrainDist;
        do {
            pos = RandomVector2D(0.0f, TerrainConsts::RANGE);
            terrainDist = terrain_->GetDistance(pos);
        } while(terrainDist.x > -100.0f);
        cometTransform.position = vec3(pos.x, terrainDist.y, pos.y);

        Comet& comet = (Comet&)entities_->CreateEntity(Comet::TYPEID, cometTransform);
        comet.seeds_ = seeds_;
        comet.spawner_ = this;

        numComets_++;
        totalComets_++;
        if (numComets_ >= simulComets_)
            doTimer_ = false;
    }
}

void CometSpawner::OnCometDestroyed() {
    numComets_--;
    if (totalComets_ < maxComets_)
        doTimer_ = true;
}

int CometSpawner::GetSeeds() {
    return maxComets_ * seeds_;
}
