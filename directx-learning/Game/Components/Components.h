#pragma once
#include "ColliderComponents.h"
#include "TransformComponents.h"
#include "StaticModelComponents.h"
#include "DesiredMovementComponents.h"
#include "GroundTraceComponents.h"
#include "VelocityComponents.h"

extern int componentCounter;
template <class T>
int GetComponentID() {
    static int componentId = componentCounter++;
    return componentId;
}
