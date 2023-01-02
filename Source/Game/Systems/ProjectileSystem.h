#include "../Components/ProjectileComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/TransformComponent.h"
#include "../World.h"

namespace ProjectileSystem {
    void CalculateVelocities(
        Entity* entities,
        ProjectileComponent& projectileComponent,
        VelocityComponent& velocityComponent,
        TransformComponent& transformComponent,
        World* world
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
