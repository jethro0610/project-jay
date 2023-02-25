#pragma once
#include "../Entity/EntityManager.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Components/SpreadDetectComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/GroundTraceComponent.h"
#include "../../Game/SpreadManager.h"
#include "../../Game/World.h"

namespace SpreadActivatorSystem {
    void Execute(
        EntityManager& entities,
        World& world,
        SpreadManager& spreadManager, 
        SpreadActivatorComponent& spreadActivatorComponent, 
        SpreadDetectComponent& spreadDetectComponent,
        TransformComponent& transformComponent,
        GroundTraceComponent& groundTraceComponent
    );
}

