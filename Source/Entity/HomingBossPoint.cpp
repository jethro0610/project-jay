#include "HomingBossPoint.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Terrain/Terrain.h"
#include "Entity/EntityList.h"
using namespace glm;

EntityDependendies HomingBossPoint::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HomingBossPoint::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_radius", &negative_->outerRadius}
        },
        {
            // Ints
        },
        {
            // Bools
        }
    };
}

void HomingBossPoint::Init(Entity::InitArgs args) {
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
    hurtbox_.radius = 2.0f;

    negative_ = terrain_->CreateNegative();
    negative_->outerRadius = 300.0f;
    negative_->sharpness = 0.25f;

    bubble_ = terrain_->CreateBubble();
    bubble_->inpower = 2.0f;
    bubble_->outpower = 32.0f;
    fader_ = DynamicFader();
    fader_.deactivateLength_ = 60 * 3;
    activateTicks_ = 0;
    containedEntities_.clear();

    SetFlag(EF_RecieveHits, false);
}

void HomingBossPoint::ActivateInTicks(int ticks) {
    if (ticks <= 0)
        Activate();
    else
        activateTicks_ = ticks;
}

void HomingBossPoint::Start() {
    negative_->position = transform_.position;
    bubble_->position = transform_.position;
    bubble_->radius = negative_->outerRadius;
    fader_.AddModifier(negative_, &negative_->innerRadius, negative_->outerRadius, -20.0f, false, EaseType::E_InQuad, -negative_->outerRadius);
    fader_.AddModifier(bubble_, &bubble_->height, 0.0f, -400.0f, true, EaseType::E_InElastic);
    fader_.ActivateModifiers(true);
    hide_ = true;

    float rad2 = negative_->outerRadius * negative_->outerRadius;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];        
        if (&entity == this)
            continue;
        if (!entity.alive_)
            continue;
        
        float dist2 = distance2(transform_.position, entity.transform_.position);
        if (dist2 < rad2) {
            entityScales_[containedEntities_.size()] = entity.transform_.scale;
            containedEntities_.push_back(&entity);
        }
    }
}

void HomingBossPoint::LateStart() {
    for (Entity* entity : containedEntities_)
        entity->Sleep();
}

void HomingBossPoint::Activate() {
    SetFlag(EF_RecieveHits, true);
    materials_[0].specularProperties.color = vec4(1.0f, 0.5f, 1.0f, 1.0f);
    fader_.StartDeactivate();
}

void HomingBossPoint::OnDestroy() {
    terrain_->FreeBubble(bubble_);
    terrain_->FreeNegative(negative_);
}

void HomingBossPoint::Update() {
    if (activateTicks_ > 0) {
        activateTicks_--;
        if (activateTicks_ == 0)
            Activate();
    }
    fader_.Update();

    if (fader_.doDeactivate_) {
        for (int i = 0; i < containedEntities_.size(); i++) {
            Entity* entity = containedEntities_[i];
            entity[i].Wake();
            float distFromTerrain = terrain_->GetDistance(entity->transform_.position).x / 50.0f;
            distFromTerrain = clamp(-distFromTerrain, 0.0f, 1.0f);
            entity->transform_.scale = entityScales_[i] * distFromTerrain;
        }
    }
}

void HomingBossPoint::EditorUpdate() {
    negative_->position = transform_.position;
    negative_->active = !DBG_preview_;
}
