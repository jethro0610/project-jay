#pragma once
#include "EntityKey.h"
#include "Debug/DebugName.h"
#ifdef _DEBUG
#include "Collision/Collider.h"
#endif
#include <vector_const.h>

const int MAX_ENTITY_EMITTERS = 8;
class ParticleEmitter;

class Entity {
public:
    USE_DEBUG_NAME;
    bool alive_;
    bool spawnedThisTick_;
    EntityKey key_;
    int stunTimer_;
    int seedsOnDestroy_;
    int seedsRadius_;
    vector_const<ParticleEmitter*, MAX_ENTITY_EMITTERS> emitters_;
    #ifdef _DEBUG
    Collider DBG_collider;
    bool DBG_selected;
    #endif

    Entity() {
        alive_ = false;
        key_ = 0;
        stunTimer_ = 0;
        spawnedThisTick_ = false;
        seedsOnDestroy_ = 0;
        seedsRadius_ = 0;
    }

    bool ShouldUpdate() const {
        return (alive_ && stunTimer_ == 0);
    }

    bool ShouldUpdate(EntityKey key) const {
        return (alive_ && stunTimer_ == 0 && MatchesKey(key));
    }

    bool MatchesKey(EntityKey key) const {
        return (key_ & key) == key;
    }

    void AddComponentById(int componentId) {
        uint32_t componentBit = 1UL << componentId; 
        key_ |= componentBit;
    }

    template <class T>
    void AddComponent() {
        AddComponentById(T::GetID());
    }
};
