#include "SpawnMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityList.h"
#include "Level/Level.h"
#include "Resource/DependencyList.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityTypes.h"
#include "EditorTextInput.h"
#include "EditorNotification.h"

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
    const std::string entityName = "e_" + textInput_.Get();

    Entity::TypeID typeId = -1;
    #define ENTITYEXP(TYPE, VAR) if (entityName == TYPE::GetName()) typeId = TYPE::GetTypeID();
    EXPANDENTITIES
    #undef ENTITYEXP

    if (typeId == -1) {
        notificaiton_.Set(entityName + " does not exist");
        return CB_Stay;
    }

    DependencyList deps;
    DependencyList::GetFromEntity(typeId, deps);
    resourceManager_.LoadDependencies(deps);

    Entity& entity = entities_.CreateEntity(typeId);
    entity.transform_.position = camera_.transform_.position + camera_.transform_.GetForwardVector() * 20.0f;

    return CB_Default;
}

void SpawnMode::Update() {
    textInput_.ReadInput();
}
