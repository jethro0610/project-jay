class Entity;
class EntityManager;
class SeedManager;
class MeterComponent;
class TransformComponent;

namespace DestroyMeterSystem {
    void Execute(
        Entity* entities,
        EntityManager& entityManager,
        SeedManager& seedManager,
        MeterComponent& meterComponent,
        TransformComponent& transformComponent
    );
}
