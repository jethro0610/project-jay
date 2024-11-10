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
    TC_FLOAT(RANGE) = 8192.0f; \
    TC_FLOAT(MESH_SIZE) = 256.0f; \
    TC_INT(RESOLUTION) = 4096; \
    TC_INT(HALF_RESOLUTION) = GET_TC_VAR(RESOLUTION) / 2; \
    TC_FLOAT(WORLD_TO_TERRAIN_SCALAR) = GET_TC_VAR(RESOLUTION) / GET_TC_VAR(RANGE); \
    TC_INT(RESOLUTION_LOW) = 256; \
    TC_INT(HALF_RESOLUTION_LOW) = GET_TC_VAR(RESOLUTION_LOW) / 2; \
    TC_FLOAT(WORLD_TO_TERRAIN_SCALAR_LOW) = GET_TC_VAR(RESOLUTION_LOW) / GET_TC_VAR(RANGE); \
    TC_INT(NUM_LODS) = 4;

#ifdef SHARED_SHADER
USE_TERRAIN_CONSTANTS
#else
namespace TerrainConsts {
    USE_TERRAIN_CONSTANTS
}
#endif
