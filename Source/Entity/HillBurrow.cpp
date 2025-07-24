#include "HillBurrow.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"
#include "Player.h"
using namespace glm;

EntityDependendies HillBurrow::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties HillBurrow::GetStaticProperties() {
    return {
        {
            // Floats
            {"p_radius", &bubble_->radius},
            {"p_height", &maxHeight_},
        },
        {
            // Ints
            {"p_hp", &maxHp_}
        },
        {
            // Bools
        }
    };
}

void HillBurrow::Init(Entity::InitArgs args) {
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

    overlapbox_.top = 1.15f;
    overlapbox_.bottom = -0.95f;
    overlapbox_.radius = 1.15f;

    SetFlag(EF_Overlap, true);
    SetFlag(EF_SendPush, true);
    SetFlag(EF_Homeable, true);

    bubble_ = terrain_->CreateBubble();
    bubble_->radius = 100.0f;
    bubble_->height = 0.0f;
    bubble_->active = true;
    bubble_->inpower = 2.0f;
    bubble_->outpower = 4.0f;
    bubble_->position = transform_.position;
    timer_ = 0;

    maxHeight_ = 100.0f;
    maxHp_ = 3;
    hp_ = 3;
}

void HillBurrow::Start() {
    hp_ = maxHp_;
    boblessHeight_ = bubble_->height;
}

void HillBurrow::PreUpdate() {
    bubble_->position = transform_.position; 
    float heightScalar = ((float)(maxHp_ - hp_) / maxHp_);
    float targetHeight = heightScalar * maxHeight_;
    float bobScalar = (sin(GlobalTime::GetTime()) + 1.0f) * 0.5f;
    boblessHeight_ = lerp(boblessHeight_, targetHeight, 0.05f);
    bubble_->height = boblessHeight_ + bobScalar * 15.0f; 
}

void HillBurrow::Update() {
    if (timer_ > 0)
        timer_--;

    transform_.position.y = terrain_->GetHeight(transform_.position);
}

void HillBurrow::EditorUpdate() {
    bubble_->position = transform_.position; 
    if (DBG_preview_)
        bubble_->height = maxHeight_;
    else
        bubble_->height = 0.0f;
}

void HillBurrow::OnDestroy() {
    terrain_->FreeBubble(bubble_);
}

void HillBurrow::OnOverlap(Entity* overlappedEntity) {
    if (timer_ > 0)
        return;

    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    Player* player = (Player*)overlappedEntity;
    player->EndHoming();
    overlappedEntity->skipGroundCheck_ = true;
    overlappedEntity->velocity_.y = 120.0f;
    hitlag_ = 8;
    player->hitlag_ = 8;
    stun_ = true;
    if (hp_ > 0)
        hp_--;

    timer_ = 20;
}

vec3 HillBurrow::GetTargetPoint() {
    return transform_.position + vec3(0.0f, overlapbox_.top, 0.0f) * transform_.scale.y;
}
