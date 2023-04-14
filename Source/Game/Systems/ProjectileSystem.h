#include "../Entity/EntityManager.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/TransformComponent.h"
#include "../World/World.h"

namespace ProjectileSystem {
    void CalculateVelocities(
        EntityManager& entityManager,
        World& world,
        ProjectileComponent& projectileComponent,
        VelocityComponent& velocityComponent,
        TransformComponent& transformComponent
    );

    void Launch(
        ProjectileComponent& projectileComponent,
        VelocityComponent& velocityComponent,
        TransformComponent& transformComponent,
        EntityID projectileEntity
    );

    void Throw(
        ProjectileComponent& projectileComponent,
        VelocityComponent& velocityComponent,
        TransformComponent& transformComponent,
        EntityID projectileEntity,
        EntityID throwingEntity,
        float height
    );
}
