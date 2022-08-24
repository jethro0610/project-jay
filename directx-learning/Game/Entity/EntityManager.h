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
        assert(!entities_[targetEntity].componentMask_.test(GetComponentID<T>()));

        entities_[targetEntity].componentMask_.set(GetComponentID<T>());
        return std::get<T>(components_)[targetEntity];
    }

    template<class T>
    T& GetComponent() {
        return std::get<T>(components_);
    }

private:

    std::tuple<
        TransformComponents,
        ColliderComponents,
        StaticModelComponents,
        GroundTraceComponents,
        VelocityComponents,
        DesiredMovementComponents
    > components_;
};