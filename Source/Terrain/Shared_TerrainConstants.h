#ifndef SHARED_SHADER
#pragma once
#define TC_FLOAT(varName) static constexpr float varName
#define TC_INT(varName) static constexpr int varName
#define GET_TC_VAR(varName) TerrainConsts::varName
#else
#define TC_FLOAT(varName) static const float TERRAIN_ ## varName
#define TC_INT(varName) static const int TERRAIN_ ## varName
#define GET_TC_VAR(varName) TERRAIN_ ## varName
#endif

#define USE_TERRAIN_CONSTANTS \
    TC_FLOAT(RANGE) = 1024.0f; \
    TC_FLOAT(MESH_SIZE) = 64.0f; \
    TC_FLOAT(MESH_DENSITY) = 0.5f; \
    TC_INT(SIZE) = GET_TC_VAR(MESH_SIZE) * GET_TC_VAR(MESH_DENSITY) + 1; \
    TC_INT(NUM_VERTICES) = GET_TC_VAR(SIZE) * GET_TC_VAR(SIZE); \
    TC_INT(NUM_INDICES) = (GET_TC_VAR(SIZE) - 1) * (GET_TC_VAR(SIZE) - 1) * 6; \
    TC_INT(RESOLUTION) = 1024; \
    TC_INT(HALF_RESOLUTION) = GET_TC_VAR(RESOLUTION) / 2; \
    TC_FLOAT(WORLD_TO_TERRAIN_SCALAR) = GET_TC_VAR(RESOLUTION) / GET_TC_VAR(RANGE); \
    TC_INT(RESOLUTION_LOW) = 128; \
    TC_INT(HALF_RESOLUTION_LOW) = GET_TC_VAR(RESOLUTION_LOW) / 2; \
    TC_FLOAT(WORLD_TO_TERRAIN_SCALAR_LOW) = GET_TC_VAR(RESOLUTION_LOW) / GET_TC_VAR(RANGE);

#ifdef SHARED_SHADER
USE_TERRAIN_CONSTANTS
#else
namespace TerrainConsts {
    USE_TERRAIN_CONSTANTS
}
#endif
