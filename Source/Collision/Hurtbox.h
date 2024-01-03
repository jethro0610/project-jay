#pragma once
#include "Collider.h"
#include <glm/vec3.hpp>

class Hitbox;
class Transform;

struct Hurtbox : public Collider {
    Transform* transform;

    glm::vec3 knockback;
    Hitbox* hitbox;
    bool hurt;
};
