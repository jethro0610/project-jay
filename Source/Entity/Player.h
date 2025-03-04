#pragma once
#include "Entity.h"

class Player : public Entity {
public:
    enum PlayerMoveMode {
        MM_Default,
        MM_Homing,
        MM_Spin,
        MM_Slope,
        MM_Attack,
        MM_Stun,
        MM_Air,
        MM_Activate,
        MM_ActivateEnd,
    };

    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_player"; }
    static EntityDependendies GetStaticDependencies();

    PlayerMoveMode moveMode_;
    float speed_;
    float bonus_;
    float tilt_;

    bool chargingAttack_;
    int attackActiveTimer_;
    int attackChargeAmount_;
    int lastAttackChargeAmount_;
    
    bool chargingActivate_;
    int activateChargeAmount_;
    int activateCooldown_;

    Entity* homingTarget_;
    float planarVelMagPreHoming_;

    ParticleEmitter* speedEmtter_;
    ParticleEmitter* spinEmitter_;
    ParticleEmitter* slopeEmitter_;

    void Update(); 
    void RenderUpdate();
    void OnHit(HitArgs args);
    void OnHurt(HurtArgs args);
    void OnCaptureSeed();
    void OnDestroy();
    void OnPush(glm::vec3 pushVec);

    void EndHoming();
    void Boost(float boost, float retention = 0.0f);
};
