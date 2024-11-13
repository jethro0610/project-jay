#include "CometSpawner.h"
#include "Comet.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Helpers/Random.h"
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
            {"p_min_interval", &minInterval_},
            {"p_max_interval", &maxInterval_},
        },
        {}
    };
}


void CometSpawner::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_default");

    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].castShadows = false;
    materials_[0].properties.color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    materials_[0].numTextures = 0;

    timer_ = 0;
    minInterval_ = 20;
    maxInterval_ = 60;
    interval_ = 0;
}

void CometSpawner::Update() {
    timer_++;
    if (timer_ >= interval_) {
        timer_ = 0;
        interval_ = RandomFloatRange(minInterval_, maxInterval_);

        Transform cometTransform;

        cometTransform.position = transform_.position;
        vec2 offset = RandomVector2D(spawnRadius_);
        cometTransform.position.x += offset.x;
        cometTransform.position.z += offset.y;

        entities_->CreateEntity(Comet::TYPEID, cometTransform);
    }
}
