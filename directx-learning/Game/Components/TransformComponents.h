#pragma once
#include "EntityConstants.h"
#include "../../Types/Transform.h"

struct TransformComponents {
    Transform transform[MAX_ENTITIES];

    TransformComponents() {};
    TransformComponents(const TransformComponents&) = delete;
    TransformComponents& operator=(const TransformComponents&) = delete;
};