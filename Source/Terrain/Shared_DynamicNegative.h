#ifndef DYNAMICNEGATIVE_H  
#define DYNAMICNEGATIVE_H 

#ifndef SHARED_SHADER
#include "Shared_DynamicTerrainModifier.h"
#else
#include <Shared_DynamicTerrainModifier.h>
#endif

#define DYN_NEG_OUTER_RADIUS(modifier) ((modifier)[1][0])
#define DYN_NEG_INNER_RADIUS(modifier) ((modifier)[1][1])
#define DYN_NEG_SHARPNESS(modifier) ((modifier)[1][2])

#ifndef SHARED_SHADER
struct DynamicNegative : public DynamicTerrainModifier {
    float outerRadius;
    float innerRadius;
    float sharpness;
};
#endif

#endif
