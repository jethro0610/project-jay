#pragma once
#include <glm/vec2.hpp>
#include "../Entity/EntityID.h"
#include "../Components/ComponentList.h"

class World;
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
    ParticleEmitter* cloudEmitter_;

    void Init(
        EntityID entity, 
        ComponentList& components,
        ParticleManager& particleManager,
        World& world
    );

    void Update(
        ComponentList& componentList,
        World& world
    );

    void FindNewTarget(World& world);
};
