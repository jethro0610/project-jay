#include "SnakeHead.h"
#include "SnakeSegment.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
#include "Terrain/Terrain.h"
#include "TPillar.h"
using namespace glm;

EntityProperties SnakeHead::GetStaticProperties() {
    return {
        {

        },
        {
            {"p_segments", &numSegments_}
        },
        {

        }
    };
}

EntityDependendies SnakeHead::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void SnakeHead::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    numSegments_ = 8;
    segmentDist_ = 64.0f;
    speed_ = 80.0f;
    velocity_ = transform_.GetForwardVector() * speed_;
    traceDistance_ = 100.0f;

    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_SendPush, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_StickToGround, true);
}

void SnakeHead::Start() {
    Transform segTransform = transform_;
    Entity* curTarget = this;
    for (int i = 0; i < numSegments_; i++) {
        segTransform.position -= transform_.GetForwardVector() * segmentDist_;
        segTransform.position.y = terrain_->GetHeight(segTransform.position);
        SnakeSegment& segment = (SnakeSegment&)entities_->CreateEntity(SnakeSegment::TYPEID, segTransform);
        segment.dist_ = segmentDist_;
        segment.target_ = curTarget;
        curTarget = &segment;
    }
}

void SnakeHead::Update() {
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_) continue;
        if (entity.typeId_ != TPillar::TYPEID) continue;

        float dist = distance(entity.transform_.position, transform_.position);
    }

    float distanceFromEdge = terrain_->GetDistance(transform_.position).x;
    if (distanceFromEdge > -32.0f) {
        velocity_ = mix(velocity_, terrain_->GetDirectionToEdge(transform_.position) * speed_, 0.1f);
    }
    velocity_ = mix(velocity_, normalize(velocity_) * speed_, 0.25f);
    transform_.rotation = quatLookAtRH(normalize(velocity_), Transform::worldUp);
}
