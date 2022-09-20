#pragma once
#include <bitset>
#include <algorithm>
#define MAX_ENTITIES 100
#define MAX_COMPONENT_TYPES 32
#define PLAYER_ENTITY 0
#define MAX_COMPONENT_TYPES 32

class Entity {
public:
    Entity() {
        std::fill_n(componentMask_, MAX_COMPONENT_TYPES, false); 
    }
    bool alive_;
    bool componentMask_[MAX_COMPONENT_TYPES];
    
    template <class T>
    bool HasComponent() const {
        return componentMask_[T::ID];
    }

    template <class... T>
    bool HasComponents() const {
        bool hasComponent[] = {
            (componentMask_[T::ID])...
        };

        for (int i = 0; i < sizeof...(T); i++) {
            if (!hasComponent[i])
                return false;
        }
        return true;
    }
};
