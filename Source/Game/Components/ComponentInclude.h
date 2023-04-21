#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "MovementComponent.h"
#include "GroundTraceComponent.h"
#include "VelocityComponent.h"
#include "ProjectileComponent.h"

#include "WorldColliderComponent.h"
#include "SpreadActivatorComponent.h"
#include "SpreadDetectComponent.h"

#include "BubbleComponent.h"

#include "IntervalSpawnComponent.h"

#include "StaticModelComponent.h"

#define CREATECOMPONENTVARS \
    COMPONENTVAR(TransformComponent, transformComponent_) \
    COMPONENTVAR(MovementComponent, movementComponent_) \
    COMPONENTVAR(GroundTraceComponent, groundTraceComponent_) \
    COMPONENTVAR(VelocityComponent, velocityComponent_) \
    COMPONENTVAR(ProjectileComponent, projectileComponent_) \
    COMPONENTVAR(WorldColliderComponent, worldColliderComponent_) \
    COMPONENTVAR(SpreadActivatorComponent, spreadActivatorComponent_) \
    COMPONENTVAR(SpreadDetectComponent, spreadDetectComponent_) \
    COMPONENTVAR(BubbleComponent, bubbleComponent_) \
    COMPONENTVAR(IntervalSpawnComponent, intervalSpawnComponent_) \
    COMPONENTVAR(StaticModelComponent, staticModelComponent_)
