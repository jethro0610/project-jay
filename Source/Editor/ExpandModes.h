#pragma once
#define EXPANDMODES \
    MODEEXPANSION(CloneMode, cloneMode_) \
    MODEEXPANSION(DefaultMode, defaultMode_) \
    MODEEXPANSION(FindMode, findMode_) \
    MODEEXPANSION(NewLevelMode, newLevelMode_) \
    MODEEXPANSION(OpenLevelMode, openLevelMode_) \
    MODEEXPANSION(RotateMode, rotateMode_) \
    MODEEXPANSION(SaveMode, saveMode_) \
    MODEEXPANSION(ScaleMode, scaleMode_) \
    MODEEXPANSION(SpawnMode, spawnMode_) \
    TAILMODEEXPANSION(TranslateMode, translateMode_)
