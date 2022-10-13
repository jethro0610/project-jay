#pragma once
#include "WorldConstants.h"

const float SPREAD_DIST = 3.0f;
const uint16_t SPREAD_PER_CHUNK = CHUNK_SIZE / SPREAD_DIST;
const uint16_t MAX_SPREAD = SPREAD_PER_CHUNK * SPREAD_PER_CHUNK;
