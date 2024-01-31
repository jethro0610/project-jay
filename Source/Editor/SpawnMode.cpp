#include "SpawnMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityList.h"
#include "Level/Level.h"
#include "Resource/DependencyList.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityTypes.h"
#include "Terrain/Terrain.h"
#include "EditorTextInput.h"
#include "EditorTarget.h"
#include "EditorNotification.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

SpawnMode::SpawnMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
    requiresTarget_ = false;
}

void SpawnMode::OnStart() {
    EditorMode::OnStart();
    textInput_.SetLabel("e_");
}

void SpawnMode::OnEnd() {
    textInput_.Clear();
}

ConfirmBehavior SpawnMode::OnConfirm() {
    const std::string name = "e_" + textInput_.Get();

    vec3 scanPosition = camera_.transform_.position;
    float distanceFromGround = 0.0f;
    for (int i = 0; i < 32; i++) {
        distanceFromGround = abs(terrain_.GetHeight(scanPosition) - scanPosition.y);
        if (distanceFromGround < 0.5f)
            break;

        scanPosition += camera_.transform_.GetForwardVector() * distanceFromGround;
    }
    if (distanceFromGround > 0.5f || any(isnan(scanPosition)))
        scanPosition = camera_.transform_.position + camera_.transform_.GetForwardVector() * 20.0f;

    if (name == "e_bubble") {
        TerrainBubble* bubble = terrain_.AddBubble(scanPosition);
        target_.SetBubble(bubble);
        // terrain_.GenerateTerrainMap(false, &entities_);
        return CB_Default;
    }
    else if (name == "e_curve") {
        TerrainCurve* curve = terrain_.AddCurve(scanPosition);
        target_.SetCurve(curve, 0);
        // terrain_.GenerateTerrainMap(false, &entities_);
        return CB_Default;
    }

    Entity::TypeID typeId = -1;
    #define ENTITYEXP(TYPE, VAR, ID) if (name == TYPE::GetName()) typeId = ID;
    EXPANDENTITIES
    #undef ENTITYEXP

    if (typeId == -1) {
        notificaiton_.Set(name + " does not exist");
        return CB_Stay;
    }

    DependencyList deps;
    DependencyList::GetFromEntity(typeId, deps);
    resourceManager_.LoadDependencies(deps);

    Transform transform;
    transform.position = scanPosition;
    Entity& entity = entities_.CreateEntity(typeId, transform);
    target_.SetEntity(&entity);

    return CB_Default;
}

void SpawnMode::Update() {
    textInput_.ReadInput();
}
