#pragma once
#include <tuple>
#include <inttypes.h>
#define CREATECOMPONENTVARS \
    COMPONENTVAR(TransformComponent, transformComponent) \
    COMPONENTVAR(MovementComponent, movementComponent) \
    COMPONENTVAR(GroundTraceComponent, groundTraceComponent) \
    COMPONENTVAR(VelocityComponent, velocityComponent) \
    COMPONENTVAR(ProjectileComponent, projectileComponent) \
    COMPONENTVAR(SpreadActivatorComponent, spreadActivatorComponent) \
    COMPONENTVAR(SpreadDetectComponent, spreadDetectComponent) \
    COMPONENTVAR(TrampleComponent, trampleComponent) \
    COMPONENTVAR(HitboxComponent, hitboxComponent) \
    COMPONENTVAR(HurtboxComponent, hurtboxComponent) \
    COMPONENTVAR(PushboxComponent, pushboxComponent) \
    COMPONENTVAR(IntervalSpawnComponent, intervalSpawnerComponent) \
    COMPONENTVAR(SkeletonComponent, skeletonComponent) \
    COMPONENTVAR(StaticModelComponent, staticModelComponent) \
    COMPONENTVAR(MeterComponent, meterComponent)

#define COMPONENTVAR(TYPE, VAR) class TYPE;
    CREATECOMPONENTVARS  
#undef COMPONENTVAR

typedef
std::tuple<
    #define COMPONENTVAR(TYPE, VAR) TYPE&,
        CREATECOMPONENTVARS  
    #undef COMPONENTVAR
    uint8_t
> ComponentList;
