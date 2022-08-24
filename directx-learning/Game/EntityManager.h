#pragma once
#include "Components/Components.h"
#include "EntityConstants.h"
#include <bitset>
#include <tuple>
#include <unordered_map>

class EntityManager {
public:
    Entity entities_[MAX_ENTITIES];

    int CreateEntity();
    //void RegisterComponent(int targetEntity, int componentId);

    template<class T>
    auto RegisterComponent(int targetEntity) {
        assert(entities_[targetEntity].alive);
        assert(!entities_[targetEntity].componentMask.test(T::ID));

        entities_[targetEntity].componentMask.set(T::ID);
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