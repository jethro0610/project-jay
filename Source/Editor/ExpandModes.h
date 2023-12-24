#pragma once
#define EXPANDMODES \
    MODEEXPANSION(BlobEditMode, blobEditMode_) \
    MODEEXPANSION(CloneMode, cloneMode_) \
    MODEEXPANSION(DefaultMode, defaultMode_) \
    MODEEXPANSION(NewLevelMode, newLevelMode_) \
    MODEEXPANSION(OpenLevelMode, openLevelMode_) \
    MODEEXPANSION(RotateMode, rotateMode_) \
    MODEEXPANSION(SaveMode, saveMode_) \
    MODEEXPANSION(ScaleMode, scaleMode_) \
    MODEEXPANSION(SpawnMode, spawnMode_) \
    MODEEXPANSION(TerrainEditMode, terrainEditMode_) \
    TAILMODEEXPANSION(TranslateMode, translateMode_)
