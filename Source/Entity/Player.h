#pragma once
#include "Entity.h"

class Player : public Entity {
public:
    enum PlayerMoveMode {
        MM_Default,
        MM_Spin,
        MM_Slope,
        MM_Attack,
        MM_Stun,
        MM_Air,
        MM_Jump,
        MM_Target
    };

    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_player"; }
    static EntityDependendies GetStaticDependencies();

    PlayerMoveMode moveMode_;
    float speed_;
    float bonus_;
    float tilt_;

    float meter_;
    float spreadRadius_;

    static constexpr int JUMP_BUFFER_FRAMES = 5;
    float jumpBuffer_[JUMP_BUFFER_FRAMES];
    int coyoteAirTime_;
    float jumpCharge_;
    bool chargingJump_;

    bool charging_;
    int attackActiveTimer_;
    int attackCharge_;
    int lastAttackCharge_;

    Entity* homingTarget_;
    float planarVelocityBeforeHoming_;

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
