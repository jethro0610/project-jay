#pragma once
#define WORLD_RESOLUTION 24
#define VOXELS_PER_COORDINATE (WORLD_RESOLUTION * WORLD_RESOLUTION * WORLD_RESOLUTION)
#define WORLD_COMPUTE_GROUPS (WORLD_RESOLUTION / 8)

#define DISTANCE_CACHE_SIZE (WORLD_RESOLUTION + 1)
#define COORDINATE_SIZE 32.0f
#define VOXEL_SIZE (COORDINATE_SIZE / (WORLD_RESOLUTION - 1))

#define MAX_X_COORDINATES 16
#define MAX_Y_COORDINATES 1
#define MAX_Z_COORDINATES 16

#define MAX_COORDINATE_VERTICES 4096
#define MAX_COORDINATE_INDICES 16384