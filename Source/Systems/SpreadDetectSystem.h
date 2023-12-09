#pragma once
#include "Systems/SystemInc.h"
class SpreadManager;

namespace SpreadDetectSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        SpreadManager& spreadManager
    );
}
