#pragma once
#include "../Components/ActiveEntityComponents.h"
#include "../Components/TransformComponents.h"
#include "../TimeConstants.h"

namespace TransformSystem {
    void UpdateLastTransforms(ActiveEntityComponents& activeEntityComponents, TransformComponents& transformComponents);
    void UpdateRenderTransforms(float interpTime, ActiveEntityComponents& activeEntityComponents, TransformComponents& transformComponents);
}