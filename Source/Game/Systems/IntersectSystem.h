#include "../../Types/EntityTypes.h"
class EntityManager;
class SpreadManager;

namespace IntersectSystem {
    void Execute(EntityManager& entityManager, SpreadManager& spreadManager); 

    void HandleIntersection(
        EntityManager& entityManager,
        SpreadManager& spreadManager,
        EntityID entity1,
        EntityID entity2 
    );
};
