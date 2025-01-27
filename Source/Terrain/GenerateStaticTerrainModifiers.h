#include "StaticTerrainModifier.h"
#include "TerrainBubble.h"
#include "TerrainNoise.h"

#define EXPANDTERRAINMODS\
	TERRAINMOD(TerrainBubble, DBG_bubbles_) \
	TERRAINMOD(TerrainNoise, DBG_noises_)
