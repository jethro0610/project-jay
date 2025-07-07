#include "PopInCrystal.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityList.h"
#include "Resource/ResourceManager.h"
using namespace glm;

EntityDependendies PopInCrystal::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties PopInCrystal::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_radius", &negative_->outerRadius},
            {"p_sharpness", &negative_->sharpness}
        },
        {
            // Ints
            {"p_breaks", &maxBreaks_}
        },
        {
            // Bools
        }
    };
}

void PopInCrystal::Init(Entity::InitArgs args) {
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

    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;
    overlapbox_.radius = 1.5f;

    negative_ = terrain_->CreateNegative();
    negative_->innerRadius = 0.0f;
    negative_->outerRadius = 100.0f;
    negative_->sharpness = 0.05f;
    numBreaks_ = 0;
    maxBreaks_ = 1;

    fader_ = DynamicFader();
    fader_.activateLength_ = 40.0f;
    fader_.deactivateLength_ = 120.0f;

    containedEntities_.clear();
    distances_.clear();
}

void PopInCrystal::Start() {
    fader_.AddModifier(
        negative_, 
        &negative_->innerRadius, 
        negative_->outerRadius, 
        -negative_->outerRadius, 
        false, 
        EaseType::E_Cubic,
        -negative_->outerRadius
    );
    fader_.ActivateModifiers(true);
    negative_->position = transform_.position;

    float outer2 = negative_->outerRadius * negative_->outerRadius;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (&entity == this)
            continue;
        if (!entity.alive_)
            continue;

        float dist2 = distance2(entity.transform_.position, transform_.position);
        if (dist2 < outer2) {
            containedEntities_.push_back(&entity);
            distances_.push_back(dist2);
            entity.Sleep();
        }
    }
}

void PopInCrystal::PreUpdate() {
    fader_.Update();

    if (negative_->active && negative_->innerRadius > 0.0f) {
        float inner2 = negative_->innerRadius * negative_->innerRadius;
        for (int i = 0; i < containedEntities_.size(); i++) {
            if (distances_[i] < inner2)
                containedEntities_[i]->Wake();
        }
    }
}

void PopInCrystal::OnDestroy() {
    terrain_->FreeNegative(negative_);
}

void PopInCrystal::Break() {
    numBreaks_++;
    if (numBreaks_ >= maxBreaks_) 
        fader_.StartDeactivate();
}

void PopInCrystal::EditorUpdate() {
    negative_->position = transform_.position;
    negative_->active = DBG_preview_;
    hitlag_ = 16;
}
