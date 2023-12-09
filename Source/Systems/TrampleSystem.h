#pragma once
#include "Systems/SystemInc.h"
class SpreadManager;

namespace TrampleSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        SpreadManager& spreadManager
    );
}
