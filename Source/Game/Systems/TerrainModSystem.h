#pragma once
#include "../Entity/Entity.h"
#include "../Components/TerrainModComponent.h"

namespace TerrainModSystem {
    void Execute(
        Entity* entities,
        TerrainModComponent& terrainModComponent
    );
}
