#pragma once
#define EXPANDMODES \
    MODEEXPANSION(CameraMode, cameraMode_) \
    MODEEXPANSION(CursorMode, cursorMode_) \
    MODEEXPANSION(PlanarScaleMode, planarScaleMode_) \
    MODEEXPANSION(PlanarTranslateMode, planarTranslateMode_) \
    MODEEXPANSION(SpawnMode, spawnMode_) \
    MODEEXPANSION(TerrainTranslateMode, terrainTranslateMode_) \
    MODEEXPANSION(UniformScaleMode, uniformScaleModee_) \
    MODEEXPANSION(VerticalScaleMode, verticalScaleMode_) \
    TAILMODEEXPANSION(VerticalTranslateMode, verticalTranslateMode_)
