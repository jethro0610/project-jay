#pragma once
#include "SystemInc.h"
class SpreadManager;

namespace SpreadDetectSystem {
    void Execute(
        EntityList& entities,
        ComponentList& components,
        SpreadManager& spreadManager
    );
}
