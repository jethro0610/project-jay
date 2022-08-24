#include "Components.h"
#define ASSIGN_COMPONENT_ID(componentType) int componentType::ID = componentCounter++
static int componentCounter = 0;

ASSIGN_COMPONENT_ID(ColliderComponents);
ASSIGN_COMPONENT_ID(DesiredMovementComponents);
ASSIGN_COMPONENT_ID(GroundTraceComponents);
ASSIGN_COMPONENT_ID(StaticModelComponents);
ASSIGN_COMPONENT_ID(TransformComponents);
ASSIGN_COMPONENT_ID(VelocityComponents);