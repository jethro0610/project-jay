#include "EntityList.h"
#include "EntityUnion.h"
#include <cstring>

EntityList::EntityList(
    EntityUnion* rawEntities,
    ParticleManager& particleManager,
    ResourceManager& resourceManager,
    Camera& camera,
    Inputs& inputs,
    Terrain& terrain
):
particleManager_(particleManager),
resourceManager_(resourceManager)
{
    rawEntities_ = rawEntities;
    for (int i = 0; i < 128; i++) {
        rawEntities[i].entity.Construct(camera, inputs, terrain);
        available_[i] = i;
        #ifdef _DEBUG
        rawEntities_[i].entity.DBG_index_ = i;
        #endif
    }

    availablePos_ = 0;
}

EntityS& EntityList::operator[](int index) {
    return rawEntities_[index].entity;
}

EntityS& EntityList::CreateEntity(EntityS::TypeID typeId) {
    int entityId = available_[availablePos_];
    available_[availablePos_] = -1;
    availablePos_++;

    switch(typeId) {
        #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): rawEntities_[entityId].VAR.Init({particleManager_, resourceManager_}); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }

    #ifdef _DEBUG
    switch(typeId) {
        #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): strncpy(rawEntities_[entityId].entity.DBG_name_, TYPE::GetName(), EntityS::MAX_NAME); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    #endif
    rawEntities_[entityId].entity.typeId_ = typeId;
    return rawEntities_[entityId].entity;
}   
