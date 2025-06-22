#pragma once
#include "ConfirmBehavior.h"
#include <glm/vec3.hpp>
#include <string>
#include <vector>

class Camera;
class EntityList;
class Inputs;
class Level;
class LevelController;
class Platform;
class Renderer;
class ResourceManager;
class SpreadManager;
class Terrain;
class TerrainCursor;
class Text;

class EditorNotification;
class EditorTargetController;
class EditorTarget;
class EditorTextInput;

struct EditorModeArgs {
    Camera& camera;
    EntityList& entities;
    Inputs& inputs;
    Level& level;
    LevelController& levelController;
    Platform& platform;
    Renderer& renderer;
    ResourceManager& resourceManager;
    SpreadManager& spreadManager;
    Terrain& terrain;
    TerrainCursor& terrainCursor;
    Text& modeText;
    EditorNotification& notificaiton;
    EditorTargetController& target;
    std::vector<EditorTarget*>& targets;
    EditorTextInput& textInput;
};

class EditorMode {
public:
    EditorMode(EditorModeArgs args);
    virtual void OnStart();
    virtual void OnEnd() { };
    virtual ConfirmBehavior OnConfirm() { return CB_Default; };
    virtual ConfirmBehavior PostConfirm() { return CB_Default; };
    virtual void OnCancel() { };
    virtual void Update() { };
    virtual std::string GetName() { return "Error"; }
    virtual int GetBinding() { return '`'; }
    bool requiresTarget_;
    bool requiresLevel_;
    bool requiresClone_;
    bool ctrl_;
    bool mouseVisibile_;

    bool CanSwitch(bool holdingCtrl) const;
    glm::vec3 GetMouseDirection();

protected:
    Camera& camera_;
    EntityList& entities_;
    LevelController& levelController_;
    Inputs& inputs_;
    Platform& platform_;
    Renderer& renderer_;
    ResourceManager& resourceManager_;
    SpreadManager& spreadManager_;
    Terrain& terrain_;
    TerrainCursor& terrainCursor_;

    Text& modeText_;
    Level& level_;
    EditorNotification& notificaiton_;
    EditorTargetController& target_;
    std::vector<EditorTarget*>& targets_;
    EditorTextInput& textInput_;
};
