#include "Segment.h"
#include "Resource/ResourceManager.h"
#include "Player.h"
#include "Time/Time.h"
#include "Helpers/Random.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies Segment::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties Segment::GetStaticProperties() {
    return {
        {
        },
        {
            {"p_t_seg_id", &targetSegId_},
        },
        {

        }
    };
}

void Segment::Init(Entity::InitArgs args) {
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_Overlap, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].properties.color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    overlapbox_.top = 2.0f;
    overlapbox_.bottom = 2.0f;
    overlapbox_.radius = 2.0f;
    timeOffset_ = RandomFloatRange(0.0f, 60.0f);
    targetSegments_ = nullptr;
    targetSegId_ = -1;
}

void Segment::Start() {
    originalY_ = transform_.position.y;
    if (targetSegId_ == -1)
        return;

    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_) 
            continue;

        int* segId = entity.GetProperties().GetInt("p_seg_id");
        if (segId == nullptr)
            continue;
        if (*segId != targetSegId_)
            continue;

        targetSegments_ = entity.GetProperties().GetInt("p_seg_count");
        int* minSegments = entity.GetProperties().GetInt("p_seg_min");
        (*minSegments )++;
    }
}

void Segment::Update() {
    transform_.position.y = originalY_ + sin(GlobalTime::GetTime() * 1.5f + timeOffset_) * 3.0f;
}

void Segment::OnOverlap(Entity* overlappedEntity) {
    if (overlappedEntity->typeId_ != Player::TYPEID)
        return;

    overlappedEntity->hitlag_ = 4;
    hitlag_ = 4;
    stun_ = true;
}

void Segment::OnHitlagEnd() {
    destroy_ = true;

    if (targetSegId_ != -1)
        (*targetSegments_)++;
}
