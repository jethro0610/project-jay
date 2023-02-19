#pragma once
#include "Entity.h"
#include "../Components/Components.h"
#include <bitset>
#include <tuple>
#include <unordered_map>

class EntityManager {
public:
    Entity entities_[MAX_ENTITIES];
    uint16_t entityCount_ = 0; // TODO: Use stack to keep track of usable entities

    uint16_t CreateEntity();

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
        StaticModelComponent
    > components_;
};
