#pragma once
#include "EditorVisibility.h"
#include <glm/vec3.hpp>
#include <string>

class Camera;
class EntityList;
class Inputs;
class Level;
class Platform;
class Renderer;
class ResourceManager;
class SpreadManager;
class Terrain;
class TerrainCursor;
class Text;

class EditorNotification;
class EditorTarget;
class EditorTextInput;

struct EditorModeArgs {
    Camera& camera;
    EntityList& entities;
    Inputs& inputs;
    Level& level;
    Platform& platform;
    Renderer& renderer;
    ResourceManager& resourceManager;
    SpreadManager& spreadManager;
    Terrain& terrain;
    TerrainCursor& terrainCursor;
    Text& modeText;
    EditorNotification& notificaiton;
    EditorTarget& target;
    EditorTextInput& textInput;
    EditorVisibility& visibility;
};

enum ConfirmBehavior {
    CB_Default,
    CB_Stay,
    CB_PostConfirm
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
    bool requiresEntity_;
    bool requiresTarget_;
    bool requiresLevel_;
    bool ctrl_;
    bool mouseVisibile_;

    bool CanSwitch(bool holdingCtrl) const;
    glm::vec3 GetMouseRay();

protected:
    Camera& camera_;
    EntityList& entities_;
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
    EditorTarget& target_;
    EditorTextInput& textInput_;
    EditorVisibility& visibility_;
};
