#pragma once
#include "../Entity/Entity.h"
#include "../SpreadManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Components/SpreadDetectComponent.h"

namespace SpreadDetectSystem {
    void Execute(
        Entity* entities, 
        SpreadManager* spreadManager,
        TransformComponent& transformComponent,
        SpreadDetectComponent& detectComponent
    );
}

