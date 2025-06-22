#pragma once
#include "EditorNotification.h"
#include "EditorTarget.h"
#include "EditorTargetController.h"
#include "EditorTextInput.h"
#include "Modes.h"
#include "ExpandModes.h"
#include "TerrainCursor.h"
#include <vector>

class Camera;
class Inputs;
class Terrain;
class Level;
class LevelProperties;
class ParticleManager;
class Renderer;
class ResourceManager;

#ifdef _PC
#include "Platform/PC_Platform.h"
#endif

class Editor {
public:
    Editor(
        Camera& camera, 
        EntityList& entities,
        Inputs& inputs,
        Level& level,
        LevelController& levelController,
        ParticleManager& particleManager,
        Platform& platform, 
        ResourceManager& resourceManager,
        Renderer& renderer,
        SpreadManager& spreadManager,
        Terrain& terrain,
        bool& running
    );
    EditorTargetController target_;
    EditorNotification notification_;
    EditorTextInput textInput_;
    Text modeText_;
    TerrainCursor terrainCursor_;

    void StartEditing();
    void StopEditing(bool startAtCamera = false);
    bool IsActive() const { return active_; };

    void Update();
    void SetMode(EditorMode& mode);
    void SetMode(EditorMode* mode);

private:
    EditorMode* mode_;
    EditorMode* postConfirmMode_;

    std::vector<EditorMode*> modes_;
    std::vector<EditorTarget*> targets_;

    Camera& camera_;
    EntityList& entities_;
    Inputs& inputs_;
    Level& level_;
    LevelController& levelController_;
    ParticleManager& particleManager_;
    Platform& platform_;
    Renderer& renderer_;
    ResourceManager& resourceManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;
    EditorModeArgs args_;

    #define MODEEXPANSION(TYPE, VAR) TYPE VAR;
    #define TAILMODEEXPANSION(TYPE, VAR) TYPE VAR;
    EXPANDMODES 
    #undef MODEEXPANSION 
    #undef TAILMODEEXPANSION

    void FlushInputs();

    bool active_;
    bool& running_;
    Transform lastCameraTransform_;
};
