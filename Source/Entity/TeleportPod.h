#pragma once
#include "Entity.h"

class TeleportPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_teleportpod"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    static constexpr int MAX_TELEPORT_SCALE_TICKS = 15;

    bool shouldTeleport_;
    glm::vec3 scaleBeforeTeleport_;
    int teleportScaleTicks_;
    int hp_;
    int seedOnHit_;
    int seedOnDestroy_;
    Entity* lastAttacker_;

    void Update(); 
    void OnHurt(HurtArgs args);
    void OnDestroy();
    int GetSeeds();
};
