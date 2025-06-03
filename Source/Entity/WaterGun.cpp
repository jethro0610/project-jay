#include "WaterGun.h"
#include "Water.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
using namespace glm;

EntityDependendies WaterGun::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties WaterGun::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
            {"p_seeds", &numSeeds_},
            {"p_tid", &targetId_}
        },
        {
            // Bools
        }
    };
}

void WaterGun::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    target_ = nullptr;
    targetId_ = -1;
    numSeeds_ = 0;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_SendPush, true);
}

void WaterGun::Start() {
    if (targetId_ == -1)
        return;

    target_ = entities_->FindEntityById(targetId_);
}

void WaterGun::OnHurt(HurtArgs args) {
    Transform waterTransform;
    waterTransform.position = transform_.position;
    waterTransform.scale = vec3(3.0f);
    Water& water = (Water&)entities_->CreateEntity(Water::TYPEID, waterTransform);
    water.Shoot(target_->transform_.position, 250.0f);
}
