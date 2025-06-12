#pragma once
#define EXPANDMODES \
    MODEEXPANSION(CloneMode, cloneMode_) \
    MODEEXPANSION(DefaultMode, defaultMode_) \
    MODEEXPANSION(FindMode, findMode_) \
    MODEEXPANSION(LabelMode, labelMode_) \
    MODEEXPANSION(ModifyEntityMode, modifyEntityMode_) \
    MODEEXPANSION(NewLevelMode, newLevelMode_) \
    MODEEXPANSION(OpenLevelMode, openLevelMode_) \
    MODEEXPANSION(RotateMode, rotateMode_) \
    MODEEXPANSION(SaveMode, saveMode_) \
    MODEEXPANSION(ScaleMode, scaleMode_) \
    MODEEXPANSION(SetLandmapMode, setLandmapMode_) \
    MODEEXPANSION(SetLevelControllerIdMode, setLevelControllerIdMode_) \
    MODEEXPANSION(SpawnMode, spawnMode_) \
    MODEEXPANSION(GrowthMode, growthMode_) \
    MODEEXPANSION(GenericScalarMode, genericScalarMode_) \
    TAILMODEEXPANSION(TranslateMode, translateMode_)
