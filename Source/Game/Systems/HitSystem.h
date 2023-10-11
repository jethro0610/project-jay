#pragma once
#include <array>
#include <vector_const.h>
#include "../Entity/EntityID.h"
#include "../Entity/EntityLimits.h"
#include "../Collision/Collision.h"

class Entity;
class Hitbox;
class HitboxComponent;
class HurtboxComponent;
class TransformComponent;
class VelocityComponent;

struct Hit {
    EntityID hitter;
    EntityID target;
    Collision collision;
    const Hitbox* hitbox;
};

const int MAX_HITS = 128;
typedef vector_const<Hit, MAX_HITS> HitList;

namespace HitSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        HitboxComponent& hitboxComponent,
        HurtboxComponent& hurtboxComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
