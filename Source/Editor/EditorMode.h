#pragma once
#include <string>

class Camera;
class EntityManager;
class Inputs;
class LevelLoader;
class LevelProperties;
class Platform;
class Renderer;
class Terrain;
class Text;
class LevelLoader;
class ResourceManager;

class EditorLevel;
class EditorTarget;
class EditorTextInput;

struct EditorModeArgs {
    Camera& camera;
    EntityManager& entityManager;
    Inputs& inputs;
    Platform& platform;
    Renderer& renderer;
    ResourceManager& resourceManager;
    Terrain& terrain;
    Text& modeText;
    EditorLevel& level;
    EditorTarget& target;
    EditorTextInput& textInput;
};

class EditorMode {
public:
    EditorMode(EditorModeArgs args);
    virtual void OnStart();
    virtual void OnEnd() { };
    virtual bool OnConfirm() { return true; };
    virtual void OnCancel() { };
    virtual void Update() { };
    virtual std::string GetName() { return "Error"; }
    virtual int GetBinding() { return '`'; }
    bool requiresTarget_;
    bool requiresLevel_;
    bool ctrl_;
    bool mouseVisibile_;

    bool CanSwitch(bool holdingCtrl) const;

protected:
    Camera& camera_;
    EntityManager& entityManager_;
    Inputs& inputs_;
    Platform& platform_;
    Renderer& renderer_;
    ResourceManager& resourceManager_;
    Terrain& terrain_;

    Text& modeText_;
    EditorLevel& level_;
    EditorTarget& target_;
    EditorTextInput& textInput_;
};
