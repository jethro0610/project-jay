#include "../Entity/EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"

namespace VelocitySystem {
    void Apply(
        EntityManager& entityManager,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
