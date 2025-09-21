#include "BubbleEater.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Bumper.h"
using namespace glm;

EntityDependendies BubbleEater::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties BubbleEater::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_radius", &bubble_->radius},
            {"p_height", &bubble_->height},
            {"p_eatradius", &overlapbox_.radius}
        },
        {
            // Ints
            {"p_holdticks", &holdTicks_}
        },
        {
            // Bools
        }
    };
}

void BubbleEater::Init(Entity::InitArgs args) {
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

    overlapbox_.radius = 2.0f;
    overlapbox_.top = 1.0f;
    overlapbox_.bottom = 1.0f;

    bubble_ = terrain_->CreateBubble();
    bubble_->active = false;
    bubble_->radius = 100.0f;
    bubble_->height = 50.0f;
    bubble_->inpower = 2.0f;
    bubble_->outpower = 1.75f;

    holdTicks_ = 60 * 4;
    holdTimer_ = 0;
    traceDistance_ = INFINITY;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
}

void BubbleEater::Start() {
    fader_ = DynamicFader();
    fader_.AddModifier(bubble_, &bubble_->height, 0.0f, bubble_->height, false, EaseType::E_Elastic); 
    fader_.DeactivateModifiers(true);
    fader_.activateLength_ = 60.0f;
    fader_.deactivateLength_ = 60.0f;
}

void BubbleEater::PreUpdate() {
    bubble_->position = transform_.position;
}

void BubbleEater::Update() {
    fader_.Update();
    if (bubble_->active)
        holdTimer_++;

    if (holdTimer_ >= holdTicks_)
        fader_.StartDeactivate();
}

void BubbleEater::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ != Bumper::TYPEID)
        return;

    overlappedEntity->destroy_ = true;
    holdTimer_ = 0;
    fader_.StartActivate();
}

void BubbleEater::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}
