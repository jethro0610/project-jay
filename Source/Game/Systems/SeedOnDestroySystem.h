#include "SystemInc.h"
class SeedManager;

namespace SeedOnDestroySystem {
    void Execute(
        EntityList& entityList,
        ComponentList& componentList,
        DestroyList& destroyList,
        SeedManager& seedManager 
    );
}
