#include "Collider.h"

class Transform;

struct Hitbox : public Collider {
    Transform* transform;

    float horizontalKb;
    float verticalKb;
    float forwardRange;

    int hitlag;
    int damage;
    bool active;

    bool hit;
};
