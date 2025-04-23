#include "EntityList.h"
#include "EntityUnion.h"
#include "Collision/Collision.h"
#include <cstring>

EntityList::EntityList(
    EntityUnion* rawEntities,
    ParticleManager& particleManager,
    ResourceManager& resourceManager,
    Camera& camera,
    Clock& clock,
    Inputs& inputs,
    Level& level,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
):
particleManager_(particleManager),
resourceManager_(resourceManager)
{
    rawEntities_ = rawEntities;
    for (int i = 0; i < MAX; i++) {
        rawEntities[i].entity.Construct(camera, clock, *this, inputs, level, seedManager, spreadManager, terrain);
        available_[i] = i;
    }

    availablePos_ = 0;
}

Entity& EntityList::operator[](int index) {
    return rawEntities_[index].entity;
}

void EntityList::Reset() {
    for (int i = 0; i < MAX; i++) {
        available_[i] = i;

        if (rawEntities_[i].entity.alive_) {
            rawEntities_[i].entity.alive_ = false;
            rawEntities_[i].entity.DoDestroy();
        }
    }
    availablePos_ = 0;
}

void EntityList::DestroyFlaggedEntities() {
    for (int i = 0; i < MAX; i++) {
        if (!rawEntities_[i].entity.alive_) continue;
        if (!rawEntities_[i].entity.destroy_) continue;

        rawEntities_[i].entity.DoDestroy();
        rawEntities_[i].entity.alive_ = false;
        availablePos_--;
        available_[availablePos_] = i;
    }
}

Entity& EntityList::CreateEntity(TypeID typeId, const Transform& transform, bool skipStart, EntityPropertiesAssigner* propertiesAssigner) {
    int entityId = available_[availablePos_];
    available_[availablePos_] = -1;
    availablePos_++;

    rawEntities_[entityId].entity.transform_ = transform;
    rawEntities_[entityId].entity.typeId_ = typeId;
    rawEntities_[entityId].entity.Init({particleManager_, resourceManager_});
    if (propertiesAssigner != nullptr)
        rawEntities_[entityId].entity.AssignProperties(*propertiesAssigner);
    if (!skipStart)
        rawEntities_[entityId].entity.DoStart();

    #ifdef _DEBUG
    rawEntities_[entityId].entity.DBG_id_ = entityId;
    #endif

    return rawEntities_[entityId].entity;
}   

bool EntityList::IsAnyOverlapping(Entity& entity) {
    for (int i = 0; i < MAX; i++) {
        Entity& target = rawEntities_[i].entity;
        if (!target.alive_) continue;
        if (&entity == &target) continue;
        if (target.pushbox_.radius <= 0.0f) continue;

        if (Collision::GetCollision(
            entity.transform_, 
            entity.pushbox_, 
            target.transform_, 
            target.pushbox_
        ).isColliding) return true;
    }
    return false;
}
