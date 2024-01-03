#pragma once
#include "Character.h"
#include "Animation/AnimationController.h"

class Camera;
class Inputs;
class Hitbox;
class Pushbox;
class ParticleEmitter;

class Player : public Character {
public:
    enum PlayerMoveMode {
        PM_Normal,
        PM_Slope,
        PM_Spin,
        PM_Attack
    };

    static constexpr float SPEED_DECAY = 0.025f; 
    static constexpr float MIN_SPEED = 12.0f;
    static constexpr float MAX_SPEED = 120.0f;
    static constexpr float MIN_FRICTION = 0.015f;
    static constexpr float MAX_FRICTION = 0.1f;
    static constexpr float MOMENTUM_DECAY = 0.025f;
    static constexpr float FRICTION_CAP_SPEED = MAX_SPEED * 0.35f;
    static constexpr float ROTATION_SPEED = 0.25f;
    static constexpr float SPIN_ROTATION_SPEED = 0.06f;
    static constexpr float ATTACK_ROTATION_SPEED = 0.025f;

    static constexpr float SLOPE_ACCELERATION = 3.0f;
    static constexpr float SLOPE_ROTATION_SPEED = 0.02f;
    static constexpr float SLOPE_DOWN_SCALING = 3.0f;
    static constexpr float SLOPE_UP_SCALING = 1.5f;
    static constexpr float MINIMUM_SLOPE_ACCELERATION_SCALING = 0.15f;

    static constexpr int MAX_ATTACK_CHARGE = 75;
    static constexpr int STRONG_ATTACK_CHARGE = 15;
    static constexpr int ATTACK_STARTUP = 2;
    static constexpr int ATTACK_ACTIVE = 8;
    static constexpr int ATTACK_COOLDOWN = 10;
    static constexpr int TOTAL_ATTACK_TIME = ATTACK_STARTUP + ATTACK_ACTIVE + ATTACK_COOLDOWN;

    static constexpr int WEAK_ATTACK_INDEX = 0;
    static constexpr int STRONG_ATTACK_INDEX = 1;

    Player(
        Camera& camera, 
        Inputs& inputs
    );

    AnimationController animationController_;

    Hurtbox* hurtbox_;
    Pushbox* pushbox_;

    PlayerMoveMode moveMode_;
    float speed_;

    int attackCharge_;
    int attackActiveTimer_;
    int currentAttackIndex_;
    Hitbox* hitboxes_[2];

    ParticleEmitter* speedEmitter_;
    ParticleEmitter* slopeEmitter_;
    ParticleEmitter* spinEmitter_;

    bool stun_;

    void Update() override;
    void SetAnimation();
    void CalculateVelocity() override;

private:
    Camera& camera_;
    Inputs& inputs_;
};
