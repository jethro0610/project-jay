#pragma once
#include <bitset>
#include "../Components/Components.h"
#define MAX_ENTITIES 100
#define MAX_COMPONENT_TYPES 32
#define PLAYER_ENTITY 0
#define MAX_COMPONENT_TYPES 32

typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;

class Entity {
public:
    bool alive_;
    ComponentMask componentMask_;

    template <class T>
    bool HasComponent() const {
        return componentMask_.test(GetComponentID<T>());
    }
};