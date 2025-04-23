#include "AttackSegment.h"
#include "Resource/ResourceManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies AttackSegment::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

EntityProperties AttackSegment::GetStaticProperties() {
    return {
        {
            // Floats
        },
        {
            // Ints
            {"p_t_seg_id", &targetSegId_},
        },
        {
            // Bools
        }
    };
}

void AttackSegment::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].specularProperties = SpecularFresnelProperties::Default();
    materials_[0].specularProperties.color = glm::vec4(1.0f);
    materials_[0].specularProperties.color = vec4(0.5f, 1.0f, 1.0f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    hurtbox_.top = 1.0f;
    hurtbox_.bottom = 1.0f;
    hurtbox_.radius = 1.5f;

    targetSegId_ = -1;
    targetSegCount_ = nullptr;

    SetFlag(EF_RecieveHits, true);
}

void AttackSegment::Start() {
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

        targetSegCount_ = entity.GetProperties().GetInt("p_seg_count");
        int* minSegments = entity.GetProperties().GetInt("p_seg_min");
        (*minSegments)++;
    }
}

void AttackSegment::OnHitlagEnd() {
    destroy_ = true;

    if (targetSegId_ != -1)
        (*targetSegCount_)++;
}
