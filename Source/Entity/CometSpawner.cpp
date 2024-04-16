#include "CometSpawner.h"
#include "Comet.h"
#include "Resource/ResourceManager.h"
#include "Particle/ParticleManager.h"
#include "Entity/EntityList.h"
#include "Helpers/Random.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityDependendies CometSpawner::GetDeps() {
    return {
        "st_comet"
    };
}

void CometSpawner::Init(Entity::InitArgs args) {
    Entity::Init(args);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_default");

    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].castShadows = false;
    materials_[0].properties.color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    materials_[0].numTextures = 0;

    timer_ = 0;
    interval_ = 0;
}

void CometSpawner::Update() {
    static float spawnRadius = 120.0f;

    timer_++;
    if (timer_ >= interval_) {
        timer_ = 0;
        interval_ = RandomFloatRange(20, 60);

        Transform cometTransform;

        cometTransform.position = transform_.position;
        vec2 offset = RandomVector2D(spawnRadius);
        cometTransform.position.x += offset.x;
        cometTransform.position.z += offset.y;

        entities_->CreateEntity(Comet::TYPEID, cometTransform);
    }
}
