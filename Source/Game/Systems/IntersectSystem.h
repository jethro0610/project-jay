#include "../Entity/EntityManager.h"
#include "../World/SpreadManager.h"

namespace IntersectSystem {
    void Execute(
        EntityManager& entityManager,
        SpreadManager& spreadManager
    ); 

    void HandleIntersection(
        EntityManager& entityManager,
        SpreadManager& spreadManager,
        EntityID entity1,
        EntityID entity2 
    );
};
