#pragma once
#include "SystemInc.h"
#include "../Entity/EntityID.h"
class World;

namespace ProjectileSystem {
    // TODO: Maybe set a flag instead of having these two functions,
    // that way we're not calling a system in another system
    void Launch(
        ComponentList& components,
        EntityID projectile,
        EntityID target
    );

    void HandleContact(
        ComponentList& components,
        EntityID projectile,
        EntityID reciever
    );

    void Execute(
        EntityList& entities, 
        ComponentList& components
    );
}
