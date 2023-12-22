#pragma once
#define EXPANDMODES \
    MODEEXPANSION(BlobEditMode, blobEditMode_) \
    MODEEXPANSION(CloneMode, cloneMode_) \
    MODEEXPANSION(DefaultMode, defaultMode_) \
    MODEEXPANSION(RotateMode, rotateMode_) \
    MODEEXPANSION(ScaleMode, scaleMode_) \
    MODEEXPANSION(SpawnMode, spawnMode_) \
    MODEEXPANSION(TerrainEditMode, terrainEditMode_) \
    TAILMODEEXPANSION(TranslateMode, translateMode_)
