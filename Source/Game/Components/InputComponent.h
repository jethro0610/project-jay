#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

enum RecieveInput {
    None,
    Player,
};

struct InputCProperties {
    RecieveInput& recieveFrom; 
};

struct InputComponent {
    RecieveInput recieveFrom[MAX_ENTITIES];
    glm::vec3 direction[MAX_ENTITIES];
    bool action[MAX_ENTITIES];
    bool toggle[MAX_ENTITIES];

    InputComponent() {
        std::fill_n(recieveFrom, MAX_ENTITIES, RecieveInput::None);
        std::fill_n(direction, MAX_ENTITIES, glm::vec3(0.0f));
        std::fill_n(action, MAX_ENTITIES, false);
        std::fill_n(toggle, MAX_ENTITIES, false);
    };
    InputComponent(const InputComponent&) = delete;
    InputComponent& operator=(const InputComponent&) = delete;

    InputCProperties operator[](int index) {
        return InputCProperties {
           recieveFrom[index]
        };
    }

    static int ID;
};
