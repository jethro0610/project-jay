#pragma once
class Entity;
class SpreadManager;
class SpreadActivatorComponent;
class SpreadDetectComponent;
class TransformComponent;

namespace SpreadDetectSystem {
    void Execute(
        Entity* entity,
        SpreadManager& spreadManager,
        SpreadActivatorComponent& spreadActivatorComponent,
        SpreadDetectComponent& spreadDetectComponent,
        TransformComponent& transformComponent
    );
}
