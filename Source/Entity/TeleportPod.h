#pragma once
#include "Entity.h"

class TeleportPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_teleportpod"; }
    static EntityDependendies GetDeps();

    static constexpr int MAX_TELEPORT_SCALE_TICKS = 15;

    bool shouldTeleport_;
    glm::vec3 scaleBeforeTeleport_;
    int teleportScaleTicks_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt();
    void OnCaptureSeed() {};
    void OnDestroy() {};
};
