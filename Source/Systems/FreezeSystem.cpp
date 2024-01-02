#include "FreezeSystem.h"

void FreezeSytem::Execute(EntityList& entities) {
    for(int i = 0; i < MAX_ENTITIES; i++) {
        Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (entity.hitlagTimer_ == 0)
            continue;

        entity.initHitlag_ = false;
        entity.hitlagTimer_--; 
    }
}
