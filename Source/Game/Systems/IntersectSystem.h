#include "../../Types/EntityTypes.h"
class Entity;
class EntityManager;
class SpreadManager;
class BubbleComponent;
class ProjectileComponent;
class TransformComponent;
class VelocityComponent;

namespace IntersectSystem {
    void Execute(
        Entity* entities,
        SpreadManager& spreadManager,
        BubbleComponent& bubbleComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    ); 

    void HandleIntersection(
        Entity* entities,
        SpreadManager& spreadManager,
        BubbleComponent& bubbleComponent,
        ProjectileComponent& projectileComponent,
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent,
        EntityID entity1,
        EntityID entity2 
    );
};
