#pragma once
#include "Entity.h"

class BumpRat : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bumprat"; }
    static EntityDependendies GetStaticDependencies(); 

    static constexpr int BACK = 0;
    static constexpr int BODY = 1;
    static constexpr int BACKEARS = 2;
    static constexpr int FRONTEARS = 3;
    static constexpr int EYES = 4;
    static constexpr int NOSE = 5;
    static constexpr int BELLY = 6;
    static constexpr glm::vec4 BODYCOLOR = glm::vec4(0.55f, 0.65f, 1.0f, 0.5f);
    static constexpr glm::vec4 LINECOLOR = glm::vec4(0.75f, 0.85f, 1.0f, 1.0f);

    static constexpr float SPEED = 60.0f;
    static constexpr float FRICTION = 0.05f;
    static constexpr float SPEED_DECAY = 1.0f - FRICTION;
    static constexpr float ACCELERATION = ((SPEED / SPEED_DECAY) - SPEED);
    static constexpr float ROTATION_SPEED = 0.25f;
    static constexpr int STARTUP_TIME = 10;

    glm::vec3 desiredMovement_;
    int attackStartup_;
     
    void Update(); 
    void OnHit(HitArgs args);
    void OnHurt(HurtArgs args);
};
