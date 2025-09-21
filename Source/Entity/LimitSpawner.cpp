#include "LimitSpawner.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies LimitSpawner::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties LimitSpawner::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_spawnscale", &spawnScale_}
        },
        {
            // Ints
            {"p_type", &spawnTypeId_}
        },
        {
            // Bools
        }
    };
}

void LimitSpawner::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.0f;

    spawnTypeId_ = -1;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
}

void LimitSpawner::OnHurt(HurtArgs args) {
    Spawn();
}

void LimitSpawner::Spawn() {
    if (spawnTypeId_ == -1)
        return;

    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_)
            continue;
        if (entity.typeId_ == spawnTypeId_)
            entity.destroy_ = true;
    }
    Transform spawnTransform;
    spawnTransform.position = transform_.position + transform_.GetForwardVector() * 20.0f;
    spawnTransform.scale = vec3(spawnScale_);
    entities_->CreateEntity(spawnTypeId_, spawnTransform);
}
