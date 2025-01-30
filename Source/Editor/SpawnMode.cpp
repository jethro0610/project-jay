#include "SpawnMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityList.h"
#include "Level/Level.h"
#include "Resource/DependencyList.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "EditorTextInput.h"
#include "EditorTargetController.h"
#include "EditorNotification.h"
#include "Entity/Entity.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

SpawnMode::SpawnMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
    requiresTarget_ = false;
    ctrl_ = true;
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

    TypeID entityId = Entity::GetTypeIDFromName(name);
    int modifierId = StaticTerrainModifier::GetIDFromName(name);

    if (entityId != - 1) {
        DependencyList deps;
        DependencyList::GetFromEntity(entityId, deps);
        resourceManager_.LoadDependencies(deps);

        Transform transform;
        transform.position = scanPosition;
        Entity& entity = entities_.CreateEntity(entityId, transform, true);
        target_.Select(entity.editorTarget_);
    }
    else if (modifierId != -1) {
        StaticTerrainModifier& modifier = terrain_.CreateStaticModifier(modifierId, scanPosition);
        target_.Select(modifier.editorTargets_[0]);
        terrain_.GenerateTerrainHeights(false, &entities_);
    }
    else {
        notificaiton_.Set(name + " does not exist");
        return CB_Stay;
    }

    return CB_Default;
}

void SpawnMode::Update() {
    textInput_.ReadInput();
}
