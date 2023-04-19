#pragma once
class EntityManager;

namespace TransformSystem {
    void UpdateLastTransforms(EntityManager& entityManager);
    void UpdateRenderTransforms(EntityManager& entityManager, float interpTime);
}
