#pragma once
#include <bitset>
#include <initializer_list> 
#include "../../Types/EntityTypes.h"
const uint8_t PLAYER_ENTITY = 0;

class Entity {
public:
    bool alive_;
    EntityKey key_;

    Entity() {
        alive_ = false;
        key_ = 0;
    }

    bool MatchesKey(EntityKey otherKey) const {
        return (key_ & otherKey) == otherKey;
    }

    template <class T>
    bool HasComponent() const {
        uint32_t componentBit = 1UL << T::GetID();
        return (key_ & componentBit) != 0;
    }

    void AddComponentById(uint8_t componentId) {
        uint32_t componentBit = 1UL << componentId; 
        key_ |= componentBit;
    }

    template <class T>
    void AddComponent() {
        AddComponentById(T::GetID());
    }
};
