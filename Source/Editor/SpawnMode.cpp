#include "SpawnMode.h"
#include "Camera/Camera.h"
#include "Entity/EntityManager.h"
#include "Level/LevelLoader.h"
#include "Resource/DependencyList.h"
#include "EditorTextInput.h"

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

bool SpawnMode::OnConfirm() {
    const std::string entityName = "e_" + textInput_.Get();

    if (!resourceManager_.HasEntityDescription(entityName)) {
        if (!std::filesystem::exists("entities/" + entityName + ".json"))
            return false;

        DependencyList deps = DependencyList::GenerateFromEntity(entityName);
        resourceManager_.LoadDependencies(deps);
    }

    Transform spawnTransform;
    spawnTransform.position = camera_.transform_.position + camera_.transform_.GetForwardVector() * 20.0f;
    entityManager_.spawner_.Spawn(resourceManager_.GetEntityDescription(entityName), spawnTransform);

    return true;
}

void SpawnMode::Update() {
    textInput_.ReadInput();
}
