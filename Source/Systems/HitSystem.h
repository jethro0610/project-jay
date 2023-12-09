#pragma once
#include "SystemInc.h"
#include "Collision/Collision.h"
#include <vector_const.h>

class Entity;

struct Hit {
    EntityID hitter;
    EntityID target;
    int hitboxId;
    Collision collision;
};

const int MAX_HITS = 128;
typedef vector_const<Hit, MAX_HITS> HitList;

namespace HitSystem {           
    void Execute(
        EntityList& entities,
        ComponentList& components
    );
}
