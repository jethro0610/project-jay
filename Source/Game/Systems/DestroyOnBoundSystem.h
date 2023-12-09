#include "Game/Systems/SystemInc.h"

namespace DestroyOnBoundSystem {
    const int Y_BOUND = -50;
    void Execute(
        EntityList& entities,
        ComponentList& components,
        DestroyList& destroyList
    );
}
