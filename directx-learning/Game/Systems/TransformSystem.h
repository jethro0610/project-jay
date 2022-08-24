#pragma once
#include "../EntityConstants.h"
#include "../Components/TransformComponents.h"
#include "../TimeConstants.h"

namespace TransformSystem {
    void UpdateLastTransforms(Entity* entities, TransformComponents& transformComponents);
    void UpdateRenderTransforms(float interpTime, Entity* entities, TransformComponents& transformComponents);
}