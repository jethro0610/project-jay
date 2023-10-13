#pragma once
#include <tuple>
#define EXPANDCOMPONENTS \
    COMPONENTEXPANSION(GroundTraceComponent, groundTraceComponent) \
    COMPONENTEXPANSION(HitboxComponent, hitboxComponent) \
    COMPONENTEXPANSION(HurtboxComponent, hurtboxComponent) \
    COMPONENTEXPANSION(IntervalSpawnComponent, intervalSpawnerComponent) \
    COMPONENTEXPANSION(MeterComponent, meterComponent) \
    COMPONENTEXPANSION(MovementComponent, movementComponent) \
    COMPONENTEXPANSION(ProjectileComponent, projectileComponent) \
    COMPONENTEXPANSION(PushboxComponent, pushboxComponent) \
    COMPONENTEXPANSION(SkeletonComponent, skeletonComponent) \
    COMPONENTEXPANSION(SpreadActivatorComponent, spreadActivatorComponent) \
    COMPONENTEXPANSION(SpreadDetectComponent, spreadDetectComponent) \
    COMPONENTEXPANSION(StaticModelComponent, staticModelComponent) \
    COMPONENTEXPANSION(TrampleComponent, trampleComponent) \
    COMPONENTEXPANSION(TransformComponent, transformComponent) \
    TAILEXPANSION(VelocityComponent, velocityComponent)

// Create forward declarations of the types
#define COMPONENTEXPANSION(TYPE, VAR) class TYPE;
#define TAILEXPANSION(TYPE, VAR) class TYPE;
EXPANDCOMPONENTS
#undef COMPONENTEXPANSION 
#undef TAILEXPANSION


class ComponentList {
    std::tuple<
        #define COMPONENTEXPANSION(TYPE, VAR) TYPE&,
        #define TAILEXPANSION(TYPE, VAR) TYPE&
        EXPANDCOMPONENTS
        #undef COMPONENTEXPANSION 
        #undef TAILEXPANSION
    > tuple_;

public:
    ComponentList(
        #define COMPONENTEXPANSION(TYPE, VAR) TYPE& VAR,
        #define TAILEXPANSION(TYPE, VAR) TYPE& VAR
        EXPANDCOMPONENTS
        #undef COMPONENTEXPANSION 
        #undef TAILEXPANSION
    ) 
    :tuple_(
        #define COMPONENTEXPANSION(TYPE, VAR) VAR,
        #define TAILEXPANSION(TYPE, VAR) VAR
        EXPANDCOMPONENTS
        #undef COMPONENTEXPANSION 
        #undef TAILEXPANSION
    ) {

    }
    template <class T>
    T& Get() {
        return std::get<T&>(tuple_);
    }
};
