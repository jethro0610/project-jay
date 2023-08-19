#include <array>
#include "../Entity/EntityLimits.h"
class Entity;
class EntityManager;
class SeedManager;
class MeterComponent;
class TransformComponent;

namespace DestroyMeterSystem {
    void Execute(
        std::array<Entity, MAX_ENTITIES>& entities,
        EntityManager& entityManager,
        SeedManager& seedManager,
        MeterComponent& meterComponent,
        TransformComponent& transformComponent
    );
}
