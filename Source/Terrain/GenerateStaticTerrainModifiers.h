#include "StaticTerrainModifier.h"
#include "TerrainBubble.h"
#include "TerrainNoise.h"
#include "TerrainBezier.h"

#define EXPANDTERRAINMODS\
	TERRAINMOD(TerrainBubble, DBG_bubbles_) \
	TERRAINMOD(TerrainNoise, DBG_noises_) \
	TERRAINMOD(TerrainBezier, DBG_beziers_)
