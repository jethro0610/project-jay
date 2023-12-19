#pragma once
#define EXPANDMODES \
    MODEEXPANSION(DefaultMode, defaultMode_) \
    MODEEXPANSION(ScaleMode, scaleMode_) \
    MODEEXPANSION(SpawnMode, spawnMode_) \
    TAILMODEEXPANSION(TranslateMode, translateMode_)
