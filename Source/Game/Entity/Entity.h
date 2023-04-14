#pragma once
#include <bitset>
#include <initializer_list> 
#include "../Components/Component.h"
const uint8_t PLAYER_ENTITY = 0;

typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;

class Entity {
public:
    bool alive_;
    ComponentMask componentMask_;

    Entity() {
        alive_ = false;
        componentMask_ = 0;
    }

    bool HasComponent(Component& component) const {
        return componentMask_.test(component.id);
    }

    bool HasComponents(const std::initializer_list<std::reference_wrapper<Component>> &list)const {
        for (auto component : list) {
            if (!HasComponent(component))
                return false;
        }
        return true;
    }
};
