#pragma once
#include <array>
#include <inttypes.h>
#include "Shared_TerrainConstants.h"

struct TerrainInfluence {
    float distance;
    float inverseDistance;
    float height;
};
