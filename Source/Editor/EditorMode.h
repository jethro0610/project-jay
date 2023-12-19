#pragma once
#include "Platform/PC_Platform.h"
#include "EditorTarget.h"
#include "EditorTextInput.h"
#include "Entity/EntityManager.h"
#include "Camera/Camera.h"
#include "Terrain/Terrain.h"
#include "Level/LevelLoader.h"
#include "Level/LevelProperties.h"
#include "Resource/ResourceManager.h"
#include "Text/Text.h"

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
    virtual void Update() { };
    virtual std::string GetName() { return "Error"; }
    virtual int GetBinding() { return '`'; }

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
