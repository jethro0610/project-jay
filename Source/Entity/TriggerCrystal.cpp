#include "TriggerCrystal.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies TriggerCrystal::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties TriggerCrystal::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_r_off", &colorOff_.r},
            {"p_g_off", &colorOff_.g},
            {"p_b_off", &colorOff_.b},

            {"p_r_on", &colorOn_.r},
            {"p_g_on", &colorOn_.g},
            {"p_b_on", &colorOn_.b},
        },
        {
            // Ints
            {"p_t_trig_id", &triggerEntityId_},
        },
        {
            // Bools
            {"p_oneway", &oneWay_},
            {"p_invert", &inverted_},
        }
    };
}

void TriggerCrystal::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    triggerEntityId_ = -1;
    triggerEntity_ = nullptr;
    oneWay_ = false;
    inverted_ = false;

    colorOff_ = vec3(0.5f, 0.5f, 0.5f);
    colorOn_ = vec3(0.5, 1.0f, 0.5f);

    SetFlag(EF_RecieveHits, true);
}

void TriggerCrystal::Start() {
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_) continue;

        if (i == triggerEntityId_) {
            triggerEntity_ = &entity;
            break;
        }
    }

    if (triggerEntity_ == nullptr)
        return;

    if (triggerEntity_->IsTriggered() ^ inverted_)
        materials_[0].specularProperties.color = vec4(colorOn_, 1.0f);
    else
        materials_[0].specularProperties.color = vec4(colorOff_, 1.0f);
}

void TriggerCrystal::OnHurt(HurtArgs args) {
    if (!oneWay_ || !(triggerEntity_->IsTriggered() ^ inverted_)) {
        triggerEntity_->DoTrigger();
    }

    if (triggerEntity_->IsTriggered() ^ inverted_)
        materials_[0].specularProperties.color = vec4(colorOn_, 1.0f);
    else
        materials_[0].specularProperties.color = vec4(colorOff_, 1.0f);
}
