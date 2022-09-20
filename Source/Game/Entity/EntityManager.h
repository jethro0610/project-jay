#pragma once
#include "Entity.h"
#include "../Components/Components.h"
#include <bitset>
#include <tuple>
#include <unordered_map>

class EntityManager {
public:
    Entity entities_[MAX_ENTITIES];

    int CreateEntity();

    template<class T>
    auto RegisterComponent(int targetEntity) {
        assert(entities_[targetEntity].alive_);
        assert(!entities_[targetEntity].componentMask_[T::ID]);

        entities_[targetEntity].componentMask_[T::ID] = true;
        return std::get<T>(components_)[targetEntity];
    }

    template<class T>
    T& GetComponent() {
        return std::get<T>(components_);
    }

private:

    std::tuple<
        TransformComponent,
        ColliderComponent,
        StaticModelComponent,
        GroundTraceComponent,
        VelocityComponent,
        MovementComponent,
        TerrainModComponent
    > components_;
};
