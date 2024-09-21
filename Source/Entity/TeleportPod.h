#pragma once
#include "Entity.h"

class TeleportPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_teleportpod"; }
    static EntityDependendies GetStaticDependencies();

    static constexpr int MAX_TELEPORT_SCALE_TICKS = 15;

    bool shouldTeleport_;
    glm::vec3 scaleBeforeTeleport_;
    int teleportScaleTicks_;

    void Update(); 
    void OnHurt(HurtArgs args);
};
