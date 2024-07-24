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
        MM_Item
    };

    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_player"; }
    static EntityDependendies GetDeps();

    static constexpr int BAND = 0;
    static constexpr int BODY = 1;
    static constexpr int HAIR = 2;
    static constexpr int MASK = 3;
    static constexpr int PANTS = 4;
    static constexpr int RIBBON = 5;
    static constexpr int SHIRT = 6;
    static constexpr int SLIPPERS = 7;

    static constexpr float MIN_SPEED = 12.0f;
    static constexpr float MAX_SPEED = 160.0f;
    static constexpr float MIN_FRICTION = 0.015f;
    static constexpr float MAX_FRICTION = 0.1f;
    static constexpr float MOMENTUM_DECAY = 2.0f;
    static constexpr float FRICTION_CAP = MAX_SPEED * 0.35f;
    static constexpr float DEFAULT_ROTATION_SPEED = 0.25f;
    static constexpr float SPIN_ROTATION_SPEED = 0.06f;
    static constexpr float ITEM_ROTATION_SPEED = 0.1f;
    static constexpr float SLOPE_ROTATION_SPEED = 0.02f;
    static constexpr float ATTACK_ROTATION_SPEED = 0.025f;
    static constexpr float AIR_ROTATION_SPEED = 0.015f;
    static constexpr float SLOPE_ACCELERATION = 4.0f;
    static constexpr float SLOPE_DOWN_SCALING = 4.0f;
    static constexpr float SLOPE_UP_SCALING = 1.5f;
    static constexpr float MIN_SLOPE_ACCELERATION_SCALING = 0.15f;
    static constexpr float MAX_JUMP = 45.0f;
    static constexpr float JUMP_CHARGE_SPEED = 1.0f;

    static constexpr int MAX_CHARGE = 75;
    static constexpr int STRONG_CHARGE_THRESH = 15;

    static constexpr int ATTACK_STARTUP = 2;
    static constexpr int ATTACK_ACTIVE = 13;
    static constexpr int ATTACK_COOLDOWN = 10;
    static constexpr int ATTACK_TIME = ATTACK_STARTUP + ATTACK_ACTIVE + ATTACK_COOLDOWN;

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
    void UseItem();
};
