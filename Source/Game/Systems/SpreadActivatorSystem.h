#pragma once
#include "../Entity/EntityManager.h"
#include "../../Game/SpreadManager.h"
#include "../../Game/World.h"

namespace SpreadActivatorSystem {
    void Execute(
        EntityManager& entities,
        World& world,
        SpreadManager& spreadManager
    );

    void AddSpread(
        EntityManager& entityManager,
        World& world,
        SpreadManager& spreadManager,
        EntityID entity,
        bool hasDetect,
        int16_t radius
    );

    void RemoveSpread(
        EntityManager& entityManager,
        World& world,
        SpreadManager& spreadManager,
        EntityID entity,
        int16_t radius
    );
}

