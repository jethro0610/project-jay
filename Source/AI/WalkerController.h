#pragma once
#include "Systems/SystemInc.h"
#include <glm/vec2.hpp>

class Terrain;
class ParticleEmitter;
class ParticleManager;

// Enemy will walk around for a little bit
// When spread is dectected in radius, it will bust and remove a lot
// Can only hit in cooldown state
// Will sniff then bust (its blind)

const float MIN_WALK_DISTANCE = 50.0f;
const float MAX_WALK_DISTANCE = 100.0f;

class WalkerController {
public:
    EntityID entity_;
    glm::vec2 targetPosition_;
    float stopTime_;
    // ParticleEmitter* cloudEmitter_;

    void Init(
        EntityID entity, 
        ComponentList& components,
        ParticleManager& particleManager,
        Terrain& terrain
    );

    void Update(
        ComponentList& componentList,
        Terrain& terrain 
    );

    void FindNewTarget(Terrain& terrain);
};