#pragma once
#include "Entity.h"
#include "../Components/Components.h"
#include <bitset>
#include <tuple>
#include <unordered_map>
#include <deque>

class EntityManager {
public:
    Entity entities_[MAX_ENTITIES];
    std::deque<uint16_t> usableEntities_;

    EntityManager();
    uint16_t CreateEntity();
    void DestroyEntity(uint16_t entityToDestroy);

    template<class T>
    auto RegisterComponent(uint16_t targetEntity) {
        assert(entities_[targetEntity].alive_);
        assert(!entities_[targetEntity].componentMask_.test(T::ID));

        entities_[targetEntity].componentMask_.set(T::ID);
        return std::get<T>(components_)[targetEntity];
    }

    template<class T>
    T& GetComponent() {
        return std::get<T>(components_);
    }

private:
    std::tuple<
        TransformComponent,
        MovementComponent,
        GroundTraceComponent,
        VelocityComponent,
        ProjectileComponent,
        TerrainModComponent,
        WorldColliderComponent,
        SpreadActivatorComponent,
        SpreadDetectComponent,
        BubbleComponent,
        PickupComponent,
        HoldableComponent,
        KickerComponent,
        KickableComponent,
        IntervalSpawnComponent,
        StaticModelComponent
    > components_;
};
