#pragma once
#include <array>
#include <glm/vec3.hpp>
#include <vector_const.h>
#include "../Entity/EntityID.h"
#include "../Entity/EntityLimits.h"
class Entity;
class PushboxComponent;
class TransformComponent;

struct Push {
    EntityID entity1;
    EntityID entity2;
    glm::vec3 resolutionVec;
};

const int MAX_COLLISIONS = 128;
typedef vector_const<Push, MAX_COLLISIONS> PushList;

namespace PushSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        PushboxComponent& pushboxComponent,
        TransformComponent& transformComponent
    );
};
