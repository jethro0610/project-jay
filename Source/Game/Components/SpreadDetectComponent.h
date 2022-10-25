#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

struct SpreadDetectCProperties {
    float& time;
};

struct SpreadDetectComponent {
    float time[MAX_ENTITIES];

    SpreadDetectComponent () {
        std::fill_n(time, MAX_ENTITIES, 0.0f);
    };
    SpreadDetectComponent (const SpreadDetectComponent &) = delete;
    SpreadDetectComponent & operator=(const SpreadDetectComponent &) = delete;

    SpreadDetectCProperties operator[](int index) {
        return {
            time[index]
        };
    };

    static int ID;
};
