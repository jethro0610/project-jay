#include "../Entity/EntityManager.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/TransformComponent.h"
#include "../World.h"

namespace ProjectileSystem {
    void CalculateVelocities(
        EntityManager& entityManager,
        World& world,
        ProjectileComponent& projectileComponent,
        VelocityComponent& velocityComponent,
        TransformComponent& transformComponent
    );

    void Launch(
        uint16_t projectileEntity, 
        uint16_t targetEntity,
        glm::vec3 velocity,
        ProjectileComponent& projectileComponent,
        VelocityComponent& velocityComponent,
        TransformComponent& transformComponent
    );
}
