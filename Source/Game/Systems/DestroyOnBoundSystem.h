#include "../Entity/EntityList.h"
#include "../Entity/DestroyList.h"
#include "../Components/ComponentList.h"

const int Y_BOUND = -10;

namespace DestroyOnBoundSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        DestroyList& destroyList
    );
}
