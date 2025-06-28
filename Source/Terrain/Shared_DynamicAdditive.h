#ifndef DYNAMICADDITIVE_H
#define DYNAMICADDITIVE_H 

#ifndef SHARED_SHADER
#include "Shared_DynamicTerrainModifier.h"
#else
#include <Shared_DynamicTerrainModifier.h>
#endif

#define DYN_ADD_RADIUS(modifier) ((modifier)[1][0])
#define DYN_ADD_SHARPNESS(modifier) ((modifier)[1][1])

#ifndef SHARED_SHADER
struct DynamicAdditive : public DynamicTerrainModifier {
    float radius;
    float sharpness;
};
#endif

#endif
