#pragma once
#include <stdint.h>

const uint8_t WORLD_RESOLUTION = 16;
const uint16_t VOXELS_PER_CHUNK = WORLD_RESOLUTION * WORLD_RESOLUTION * WORLD_RESOLUTION;
const uint8_t WORLD_COMPUTE_GROUPS = WORLD_RESOLUTION / 8;

const uint8_t DISTANCE_CACHE_SIZE = WORLD_RESOLUTION + 1;
const float CHUNK_SIZE = 32.0f;
const float VOXEL_SIZE = CHUNK_SIZE / (WORLD_RESOLUTION - 1);

const uint8_t MAX_X_CHUNKS = 8; 
const uint8_t MAX_Y_CHUNKS = 4;
const uint8_t MAX_Z_CHUNKS = 8; 

const uint16_t MAX_CHUNK_VERTICES = 4096;
const uint16_t MAX_CHUNK_INDICES = 16384;

const uint16_t NOISE_SIZE = 8192;
const uint16_t NOISE_DETAIL = 16;
