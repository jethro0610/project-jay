#pragma once
#include "../Entity/EntityManager.h"
#include "../Components/TerrainModComponent.h"

namespace TerrainModSystem {
    void Execute(
        EntityManager& entities,
        TerrainModComponent& terrainModComponent
    );
}
