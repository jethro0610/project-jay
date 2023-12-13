#pragma once
#include "Platform/PC_Platform.h"
#include "Entity/EntityManager.h"
#include "Camera/Camera.h"
#include "Terrain/Terrain.h"
#include "Level/LevelLoader.h"
#include "Level/LevelProperties.h"

#ifdef _PC
#include "Platform/PC_Platform.h"
#endif

enum EditorMode {
    EM_Camera,
    EM_Mouse
};

class Editor {
public:
    Editor(
        Camera& camera, 
        EntityManager& entityManager, 
        Inputs& inputs,
        LevelLoader& levelLoader,
        LevelProperties& levelProperties,
        Platform& platform, 
        Renderer& renderer,
        Terrain& terrain,
        bool& running
    );
    EditorMode mode_;
    EntityID target_;

    void StartEditing();
    void StopEditing();
    bool IsActive() const { return active_; };
    void Update();
    void SetMode(EditorMode mode);

private:
    Camera& camera_;
    EntityManager& entityManager_;
    Inputs& inputs_;
    LevelLoader& levelLoader_;
    LevelProperties& levelProperties_;
    Platform& platform_;
    Renderer& renderer_;
    Terrain& terrain_;

    void FlushInputs();

    bool active_;
    bool& running_;
};
