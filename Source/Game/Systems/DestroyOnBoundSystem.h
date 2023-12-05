#include "../Entity/EntityList.h"
#include "../Entity/DestroyList.h"
#include "../Components/ComponentList.h"


namespace DestroyOnBoundSystem {
    const int Y_BOUND = -50;
    void Execute(
        EntityList& entities,
        ComponentList& components,
        DestroyList& destroyList
    );
}
