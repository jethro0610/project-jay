#include "FreezeSystem.h"
#include "Game/Entity/EntityLimits.h"
#include "Game/Entity/Entity.h"

void FreezeSytem::Execute(EntityList& entities) {
    for(int i = 0; i < MAX_ENTITIES; i++) {
        Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (entity.stunTimer_ == 0)
            continue;

        entity.stunTimer_--; 
    }
}
