#pragma once
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
