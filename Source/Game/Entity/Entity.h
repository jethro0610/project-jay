#pragma once
#include <bitset>
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