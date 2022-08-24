#pragma once
#include "EntityConstants.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct GroundTraceCProperties {
    float& distance;
};

struct GroundTraceComponents {
    float distance[MAX_ENTITIES];
    bool onGround[MAX_ENTITIES];
    bool onGroundLastFrame[MAX_ENTITIES];
    bool enteredGround[MAX_ENTITIES];
    bool exitedGround[MAX_ENTITIES];
    vec3 groundPosition[MAX_ENTITIES];
    vec3 groundNormal[MAX_ENTITIES];

    GroundTraceComponents() {
        std::fill_n(distance, MAX_ENTITIES, 0.0f);
        std::fill_n(onGround, MAX_ENTITIES, false);
        std::fill_n(enteredGround, MAX_ENTITIES, false);
        std::fill_n(exitedGround, MAX_ENTITIES, false);
        std::fill_n(onGroundLastFrame, MAX_ENTITIES, false);
        std::fill_n(groundPosition, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(groundNormal, MAX_ENTITIES, vec3(0.0f, 0.0f, 0.0f));
    };
    GroundTraceComponents(const GroundTraceComponents&) = delete;
    GroundTraceComponents& operator=(const GroundTraceComponents&) = delete;

    GroundTraceCProperties operator[](int index) {
        return GroundTraceCProperties {
            distance[index]
        };
    }
};