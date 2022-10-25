#include "Components.h"
#define ASSIGN_COMPONENT_ID(componentType) int componentType::ID = componentCounter++
static int componentCounter = 0;

ASSIGN_COMPONENT_ID(ColliderComponent);
ASSIGN_COMPONENT_ID(MovementComponent);
ASSIGN_COMPONENT_ID(GroundTraceComponent);
ASSIGN_COMPONENT_ID(StaticModelComponent);
ASSIGN_COMPONENT_ID(TransformComponent);
ASSIGN_COMPONENT_ID(VelocityComponent);
ASSIGN_COMPONENT_ID(TerrainModComponent);
ASSIGN_COMPONENT_ID(SpreadActivatorComponent);
ASSIGN_COMPONENT_ID(SpreadDetectComponent);

// Static IDs are used on component classes for consistency with file writes
