#pragma once
#include "EntityConstants.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct TransformComponents {
    Transform transform[MAX_ENTITIES];
    Transform transformLastUpdate[MAX_ENTITIES];
    Transform renderTransform[MAX_ENTITIES];
    bool interpolate[MAX_ENTITIES];

    TransformComponents() {
        std::fill_n(interpolate, MAX_ENTITIES, false);
    };
    TransformComponents(const TransformComponents&) = delete;
    TransformComponents& operator=(const TransformComponents&) = delete;
};