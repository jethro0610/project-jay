#pragma once
#include "EntityS.h"

class Player : public EntityS {
public:
    enum PlayerMoveMode {
        MM_Default,
        MM_Spin,
        MM_Slope,
        MM_Attack
    };

    void Init(InitArgs args);

    static constexpr TypeID GetTypeID() { return 1; };
    static constexpr const char* GetName() { return "e_player"; }

    static constexpr float MIN_SPEED = 12.0f;
    static constexpr float MAX_SPEED = 120.0f;
    static constexpr float MIN_FRICTION = 0.015f;
    static constexpr float MAX_FRICTION = 0.1f;
    static constexpr float MOMENTUM_DECAY = 1.0f;
    static constexpr float FRICTION_CAP = MAX_SPEED * 0.35f;
    static constexpr float DEFAULT_ROTATION_SPEED= 0.25f;
    static constexpr float SPIN_ROTATION_SPEED = 0.06f;
    static constexpr float SLOPE_ROTATION_SPEED = 0.02f;
    static constexpr float ATTACK_ROTATION_SPEED = 0.025f;
    static constexpr float SLOPE_ACCELERATION = 3.0f;
    static constexpr float SLOPE_DOWN_SCALING = 3.0f;
    static constexpr float SLOPE_UP_SCALING = 1.5f;
    static constexpr float MIN_SLOPE_ACCELERATION_SCALING = 0.15f;
    static constexpr float TRACE_DISTANCE = 1.0f;

    PlayerMoveMode moveMode_;
    float speed_;

    ParticleEmitter* speedEmtter_;
    ParticleEmitter* spinEmitter_;
    ParticleEmitter* slopeEmitter_;
     
    void Update(); 
    void RenderUpdate();
};
