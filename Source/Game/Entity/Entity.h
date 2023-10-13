#pragma once
#include <bitset>
#include <initializer_list> 
#include "EntityKey.h"

class Entity {
public:
    bool alive_;
    EntityKey key_;
    int stunTimer_;

    Entity() {
        alive_ = false;
        key_ = 0;
        stunTimer_ = 0;
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
