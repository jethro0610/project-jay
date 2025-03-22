#include "SnakeHead.h"
#include "SnakeSegment.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
#include "Terrain/Terrain.h"
#include "TPillar.h"
#include "SnakePod.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

EntityProperties SnakeHead::GetStaticProperties() {
    return {
        {
            {"p_dist", &segmentDist_},
            {"p_segscale", &segmentScale_},
            {"p_podscale", &podScale_}
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
    segmentDist_ = 32.0f;
    speed_ = 80.0f;
    velocity_ = transform_.GetForwardVector() * speed_;
    traceDistance_ = 100.0f;

    segmentScale_ = 1.0f;
    podScale_ = 0.5f;

    SetFlag(EF_UseVelocity, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_SendPush, true);
    SetFlag(EF_RecievePush, true);
    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_StickToGround, true);
}

void SnakeHead::Start() {
    vec3 curPos = transform_.position;
    Entity* curSegment = this;
    Entity* prevSegment = this;

    DEBUGLOG(segmentDist_);
    for (int i = 0; i < numSegments_; i++) {
        prevSegment = curSegment;
        Transform segmentTransform;
        segmentTransform.scale = transform_.scale * segmentScale_;
        curPos -= transform_.GetForwardVector() * segmentDist_;
        curPos.y = terrain_->GetHeight(curPos);
        segmentTransform.position = curPos;
        SnakeSegment& segment = (SnakeSegment&)entities_->CreateEntity(SnakeSegment::TYPEID, segmentTransform);
        segment.prevSegment_ = curSegment;
        segment.distFromPrev_ = segmentDist_;
        segment.head_ = this;
        curSegment = &segment;

        Transform podTransform;
        podTransform.scale = transform_.scale * podScale_;
        podTransform.position = (prevSegment->transform_.position + curSegment->transform_.position) * 0.5f;
        SnakePod& pod = (SnakePod&)entities_->CreateEntity(SnakePod::TYPEID, podTransform);
        pod.prevSegment_ = prevSegment;
        pod.nextSegment_ = curSegment;
        pod.head_ = this;
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

    vec3 planarVelocity = velocity_;
    planarVelocity.y = 0.0f;
    transform_.rotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp);
}
