#pragma once
#include "../Entity/EntityManager.h"
#include "../SpreadManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Components/SpreadDetectComponent.h"

namespace SpreadDetectSystem {
    void Execute(
        EntityManager& entityManager,
        SpreadManager& spreadManager,
        TransformComponent& transformComponent,
        SpreadDetectComponent& detectComponent
    );
}

