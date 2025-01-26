#include "StaticTerrainModifier.h"
#include "TerrainBubble.h"
#include "TerrainNoise.h"

#define EXPANDTERRAINMODS\
	TERRAINMOD(TerrainBubble, bubbles_) \
	TERRAINMOD(TerrainNoise, noises_)
