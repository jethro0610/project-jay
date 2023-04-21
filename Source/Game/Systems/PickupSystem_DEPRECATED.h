#pragma once
class EntityManager;

namespace PickupSystem {
    bool TryPickup(EntityManager& entityManager, int pickupEntityId, int holdEntityId);
    void ExecuteHold(EntityManager& entityManager);
}
