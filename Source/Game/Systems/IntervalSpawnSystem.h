#include "../Entity/EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/IntervalSpawnComponent.h"

namespace IntervalSpawnSystem {
    void Execute (
        EntityManager& entityManager,
        TransformComponent& transformComponent,
        IntervalSpawnComponent& intervalSpawnComponent
    );
};
