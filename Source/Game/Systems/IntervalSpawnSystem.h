#include "../Entity/EntityManager.h"
#include "../Components/IntervalSpawnComponent.h"

namespace IntervalSpawnSystem {
    void Execute (
        EntityManager& entityManager,
        IntervalSpawnComponent& intervalSpawnComponent
    );
};
