#ifndef DYNAMICBUBBLE_H  
#define DYNAMICBUBBLE_H 

#ifndef SHARED_SHADER
#include "Shared_DynamicTerrainModifier.h"
#else
#include <Shared_DynamicTerrainModifier.h>
#endif

#define DYN_BUB_RADIUS(modifier) ((modifier)[1][0])
#define DYN_BUB_HEIGHT(modifier) ((modifier)[1][1])
#define DYN_BUB_INPOWER(modifier) ((modifier)[1][2])
#define DYN_BUB_OUTPOWER(modifier) ((modifier)[1][2])

#ifndef SHARED_SHADER
struct DynamicBubble: public DynamicTerrainModifier {
    float radius;
    float height;
    float inpower;
    float outpower;
};
#endif

#endif
