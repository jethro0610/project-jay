#include "../Entity/EntityID.h"
class Entity;
class EntityManager;
class SeedManager;
class SpreadManager;
class ColliderComponent;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

namespace CollisionSystem {
    void Execute(
        Entity* entities,
        EntityManager& entityManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        ColliderComponent& bubbleComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    ); 

    void HandleIntersection(
        Entity* entities,
        EntityManager& entityManager,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        ColliderComponent& bubbleComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        EntityID entity1,
        EntityID entity2 
    );
};
