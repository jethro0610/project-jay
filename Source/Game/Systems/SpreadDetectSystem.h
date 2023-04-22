#pragma once
class Entity;
class SpreadManager;
class SpreadDetectComponent;
class TransformComponent;

namespace SpreadDetectSystem {
    void Execute(
        Entity* entity,
        SpreadManager& spreadManager,
        SpreadDetectComponent& spreadDetectComponent,
        TransformComponent& transformComponent
    );
}
