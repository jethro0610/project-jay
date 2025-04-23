#include "ACrystal.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies ACrystal::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties ACrystal::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
            {"p_seg_id", &segmentId_},
            {"p_seg_count", &numSegments_},
            {"p_seg_min", &minSegments_},
            {"p_t_trig_id", &triggerEntityId_},
        },
        {
            // Bools
        }
    };
}

void ACrystal::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = vec4(1.0f, 0.5f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hasTriggered_ = false;
    segmentId_ = -1;
    numSegments_ = 0;
    minSegments_ = 0;

    triggerEntityId_ = -1;
    triggerEntity_ = nullptr;
}

void ACrystal::Start() {
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_) continue;

        if (i == triggerEntityId_) {
            triggerEntity_ = &entity;
            break;
        }
    }
}

void ACrystal::Update() {
    if (triggerEntity_ == nullptr)
        return;

    if (numSegments_ >= minSegments_ && !hasTriggered_) {
        hasTriggered_ = true;
        triggerEntity_->DoTrigger();
    }
}
