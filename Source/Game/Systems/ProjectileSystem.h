#pragma once
#include "../Entity/EntityID.h"
class Entity;
class World;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

namespace ProjectileSystem {
    // TODO: Can maybe have a separate system for halting on world
    // collision
    void CalculateVelocities(
        Entity* entities, 
        World& world,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );

    // TODO: Maybe set a flag instead of having these two functions,
    // that way we're not calling a system in another system
    void Launch(
        ProjectileComponent& projectileComponent, 
        TransformComponent& transformComponent, 
        VelocityComponent& velocityComponent, 
        EntityID projectile,
        EntityID target
    );
}
