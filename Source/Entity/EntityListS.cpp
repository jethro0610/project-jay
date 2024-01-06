#include "EntityListS.h"
#include "EntityUnion.h"

EntityListS::EntityListS(
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
    }

    availablePos_ = 0;
}

EntityS& EntityListS::operator[](int index) {
    return rawEntities_[index].entity;
}

EntityS::TypeID EntityListS::GetTypeID(EntityIDS entityId) {
    return rawEntities_[entityId].typeId;
}

EntityIDS EntityListS::CreateEntity(EntityS::TypeID typeId) {
    int entityId = available_[availablePos_];
    available_[availablePos_] = -1;
    availablePos_++;

    switch(typeId) {
        #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): rawEntities_[entityId].VAR.Init({particleManager_, resourceManager_}); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    rawEntities_[entityId].typeId = typeId;

    return entityId;
}   

void EntityListS::DestroyEntity(EntityIDS entityId) {
    assert(Valid(entityId));
    availablePos_--;
    available_[availablePos_] = entityId;
}

const char* EntityListS::GetName(EntityIDS entityId) {
    assert(Valid(entityId));
    switch(rawEntities_[entityId].typeId) {
        #define ENTITYEXP(TYPE, VAR) case TYPE::GetTypeID(): return TYPE::GetName();
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    return "e_invalid";
}
