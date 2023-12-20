#ifndef SHARED_SHADER
#define TC_FLOAT(varName) static constexpr float varName
#define TC_INT(varName) static constexpr int varName
#define GET_TC_VAR(varName) varName
#else
#define TC_FLOAT(varName) static const float TERRAIN_ ## varName
#define TC_INT(varName) static const int TERRAIN_ ## varName
#define GET_TC_VAR(varName) TERRAIN_ ## varName
#endif

#define USE_TERRAIN_CONSTANTS \
    TC_FLOAT(RANGE) = 1024.0f; \
    TC_FLOAT(MESH_SIZE) = 64.0f; \
    TC_FLOAT(MESH_DENSITY) = 0.5f; \
    TC_INT(RESOLUTION) = 4096; \
    TC_INT(HALF_RESOLUTION) = GET_TC_VAR(RESOLUTION) / 2; \
    TC_FLOAT(WORLD_TO_TERRAIN_SCALAR) = GET_TC_VAR(RESOLUTION) / GET_TC_VAR(RANGE);

#ifdef SHARED_SHADER
USE_TERRAIN_CONSTANTS
#endif
