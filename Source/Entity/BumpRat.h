#pragma once
#include "EntityS.h"

class BumpRat : public EntityS {
public:
    void Init(InitArgs args);

    static constexpr TypeID GetTypeID() { return 2; };
    static constexpr const char* GetName() { return "e_bumprat"; }
    static constexpr float SPEED = 60.0f;
    static constexpr float FRICTION = 0.05f;
    static constexpr float SPEED_DECAY = 1.0f - FRICTION;
    static constexpr float ACCELERATION = ((SPEED / SPEED_DECAY) - SPEED);
    static constexpr float ROTATION_SPEED = 0.25f;

    glm::vec3 desiredMovement_;
     
    void Update(); 
    void RenderUpdate() {};
};
