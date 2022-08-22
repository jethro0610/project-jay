#pragma once
#include "EntityConstants.h"
#include "../../Types/Transform.h"
#include <algorithm>

#define NO_GROUND_TRACE -1.0f

struct GroundTraceComponents {
    float distance[MAX_ENTITIES];
    bool onGround[MAX_ENTITIES];
    bool onGroundLastFrame[MAX_ENTITIES];
    vec3 groundPosition[MAX_ENTITIES];
    vec3 groundNormal[MAX_ENTITIES];

    GroundTraceComponents() {
        std::fill_n(distance, MAX_ENTITIES, NO_GROUND_TRACE);
        std::fill_n(onGround, MAX_ENTITIES, false);
        std::fill_n(onGroundLastFrame, MAX_ENTITIES, false);
        std::fill_n(groundPosition, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(groundNormal, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
    };
    GroundTraceComponents(const GroundTraceComponents&) = delete;
    GroundTraceComponents& operator=(const GroundTraceComponents&) = delete;
};