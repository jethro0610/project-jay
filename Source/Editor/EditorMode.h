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

class EditorTarget;
class EditorTextInput;

struct EditorModeArgs {
    Camera& camera;
    EntityManager& entityManager;
    Inputs& inputs;
    LevelLoader& levelLoader;
    LevelProperties& levelProperties;
    Platform& platform;
    Renderer& renderer;
    ResourceManager& resourceManager;
    Terrain& terrain;
    Text& modeText;
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

protected:
    bool mouseVisibile_;

    Camera& camera_;
    EntityManager& entityManager_;
    Inputs& inputs_;
    LevelLoader& levelLoader_;
    LevelProperties& levelProperties_;
    Platform& platform_;
    Renderer& renderer_;
    ResourceManager& resourceManager_;
    Terrain& terrain_;

    Text& modeText_;
    EditorTarget& target_;
    EditorTextInput& textInput_;
};
