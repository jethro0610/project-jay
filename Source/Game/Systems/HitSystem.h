#pragma once
#include <vector_const.h>
#include "SystemInc.h"
#include "Game/Collision/Collision.h"
#include "Game/Components/ComponentList.h"

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
