#pragma once
class Entity;
class TransformComponent;

namespace TransformSystem {
    void UpdateLastTransforms(
        Entity* entities,
        TransformComponent& transformComponent
    );
    void UpdateRenderTransforms(
        Entity* entities,
        TransformComponent& transformComponent,
        float interpTime
    );
}
