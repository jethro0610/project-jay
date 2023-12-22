#pragma once
#include "EditorMode.h"
#include "Entity/EntityID.h"
#include "Entity/SpawnPromise.h"
#include "Entity/EntityDescription.h"
#include "Types/Transform.h"
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

class CloneMode : public EditorMode {
    enum CloneSubmode {
        CS_Planar,
        CS_Terrain
    };
public:
    CloneMode(EditorModeArgs args);
    std::string GetName() override;
    int GetBinding() override { return 'O'; }
    void OnStart() override;
    void OnEnd() override;
    bool OnConfirm() override;
    void Update() override;

private:
    void SetSubmode(CloneSubmode);
    CloneSubmode submode_;

    float deltaX_;
    float deltaY_;

    EntityID original_;    
    EntityDescription* originalDescription_;
    Transform originalTransform_;

    SpawnPromise spawnPromise_;
};
