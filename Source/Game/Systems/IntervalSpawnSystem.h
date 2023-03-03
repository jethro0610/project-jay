#include "../Entity/EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/IntervalSpawnComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/VelocityComponent.h"

namespace IntervalSpawnSystem {
    void Execute (
        EntityManager& entityManager,
        TransformComponent& transformComponent,
        IntervalSpawnComponent& intervalSpawnComponent,
        ProjectileComponent& projectileComponent,
        VelocityComponent& velocityComponent
    );
};
