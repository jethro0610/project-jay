#pragma once
#include "Entity.h"
#include "Item.h"

class Player : public Entity {
public:
    enum PlayerMoveMode {
        MM_Default,
        MM_Spin,
        MM_Slope,
        MM_Attack,
        MM_JumpCharge,
        MM_Stun,
        MM_Air,
        MM_Item,
        MM_Target
    };

    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_player"; }
    static EntityDependendies GetDeps();

    PlayerMoveMode moveMode_;
    float speed_;
    float tilt_;

    float meter_;
    int spinTime_;

    bool charging_;
    int attackActiveTimer_;
    int attackCharge_;
    int lastAttackCharge_;

    bool chargingJump_;
    float jumpCharge_;

    Entity* homingTarget_;
    float planarVelocityBeforeHoming_;

    Item item_;
    int itemTimer_;
    int itemMoveTimer_;

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
    void OnOverlap(Entity* overlappedEntity);

    void UseItem();
};
