#pragma once
#include "Component.h"
#include <glm/vec3.hpp>

class AIComponent : public Component {
public:
    enum AIController {
        AI_Dummy,
        AI_SpinRat
    };

    std::array<AIController, MAX_ENTITIES> controller;
    std::array<glm::vec3, MAX_ENTITIES> target;
    std::array<int, MAX_ENTITIES> timer; 

    AIComponent() {
        controller.fill(AI_Dummy);
        target.fill(glm::vec3(0.0f));
        timer.fill(0);
    };
    AIComponent(const AIComponent&) = delete;
    AIComponent& operator=(const AIComponent&) = delete;

    static constexpr std::string GetName() { return "ai"; }
    static constexpr int GetID() { return 19; }

    void Load(nlohmann::json& data, EntityID entity) {
        timer[entity] = -1;

        std::string controllerName = GetString(data, "controller");
        if (controllerName == "spinrat") {
            controller[entity] = AI_SpinRat;
        }
        else
            controller[entity] = AI_Dummy;
    }
};
