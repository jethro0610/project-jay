#pragma once
#include "Entity.h"

class TrailFish : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_trailfish"; }
    static EntityDependendies GetDeps();

    static constexpr float SPEED = 80.0f;
    static constexpr float FRICTION = 0.05f;
    static constexpr float SPEED_DECAY = 1.0f - FRICTION;
    static constexpr float ACCELERATION = ((SPEED / SPEED_DECAY) - SPEED);
    static constexpr float ROTATION_SPEED = 0.25f;
    static constexpr int SPAWN_INTERVAL = 15;

    int spawnTimer_;
    glm::vec3 desiredMovement_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {};
    void OnOverlap(Entity* overlappedEntity) {};
};
