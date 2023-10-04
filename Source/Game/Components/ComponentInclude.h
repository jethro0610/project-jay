#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "MovementComponent.h"
#include "GroundTraceComponent.h"
#include "VelocityComponent.h"
#include "ProjectileComponent.h"

#include "SpreadActivatorComponent.h"
#include "SpreadDetectComponent.h"
#include "TrampleComponent.h"

#include "ColliderComponent.h"

#include "IntervalSpawnComponent.h"
#include "MeterComponent.h"

#include "SkeletonComponent.h"
#include "StaticModelComponent.h"

#define CREATECOMPONENTVARS \
    COMPONENTVAR(TransformComponent, transformComponent_) \
    COMPONENTVAR(MovementComponent, movementComponent_) \
    COMPONENTVAR(GroundTraceComponent, groundTraceComponent_) \
    COMPONENTVAR(VelocityComponent, velocityComponent_) \
    COMPONENTVAR(ProjectileComponent, projectileComponent_) \
    COMPONENTVAR(SpreadActivatorComponent, spreadActivatorComponent_) \
    COMPONENTVAR(SpreadDetectComponent, spreadDetectComponent_) \
    COMPONENTVAR(TrampleComponent, trampleComponent_) \
    COMPONENTVAR(ColliderComponent, colliderComponent_) \
    COMPONENTVAR(IntervalSpawnComponent, intervalSpawnComponent_) \
    COMPONENTVAR(SkeletonComponent, skeletonComponent_) \
    COMPONENTVAR(StaticModelComponent, staticModelComponent_) \
    COMPONENTVAR(MeterComponent, meterComponent_)
