#include "../Components/ComponentList.h"
#include "../Entity/EntityList.h"
#include "../Entity/DestroyList.h"
class SeedManager;

namespace SeedOnDestroySystem {
    void Execute(
        EntityList& entityList,
        ComponentList& componentList,
        DestroyList& destroyList,
        SeedManager& seedManager 
    );
}
