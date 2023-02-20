#pragma once
#include <bitset>
const uint16_t MAX_ENTITIES = 1024;
const uint8_t MAX_COMPONENT_TYPES = 32;
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

    template <class T>
    bool HasComponent() const {
        return componentMask_.test(T::ID);
    }

    template <class... T>
    bool HasComponents() const {
        bool hasComponent[] = {
            (componentMask_.test(T::ID))...
        };

        for (int i = 0; i < sizeof...(T); i++) {
            if (!hasComponent[i])
                return false;

        }
        return true;
    }

};
