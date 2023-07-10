#pragma once
#include "../Entity/EntityID.h"
class Entity;
class World;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

namespace ProjectileSystem {
    // TODO: Maybe set a flag instead of having these two functions,
    // that way we're not calling a system in another system
    void Launch(
        ProjectileComponent& projectileComponent, 
        TransformComponent& transformComponent, 
        VelocityComponent& velocityComponent, 
        EntityID projectile,
        EntityID target
    );

    void Execute(
        Entity* entities, 
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
