#pragma once
#include "SystemInc.h"
#include <glm/vec3.hpp>
#include <vector_const.h>
#include "Game/Collision/Collision.h"
#include "Game/Entity/EntityID.h"

class Entity;
class PushboxComponent;
class TransformComponent;

struct Push {
    EntityID entityA;
    EntityID entityB;
    Collision collision;
};

const int MAX_COLLISIONS = 128;
typedef vector_const<Push, MAX_COLLISIONS> PushList;

namespace PushSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components
    );
};
