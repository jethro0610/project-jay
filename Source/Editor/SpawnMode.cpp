#include "SpawnMode.h"
#include <GLFW/glfw3.h>

SpawnMode::SpawnMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = false;
}

void SpawnMode::OnStart() {
    EditorMode::OnStart();
    textInput_.SetLabel("Spawn Entity: ");
}

void SpawnMode::OnEnd() {
    textInput_.Clear();
}

void SpawnMode::Update() {
    textInput_.ReadInput();

    if (platform_.pressedKeys_[GLFW_KEY_ENTER]) {
        const std::string entityName = "e_" + textInput_.Get();
        textInput_.Clear();

        if (!resourceManager_.HasEntityDescription(entityName)) {
            if (!std::filesystem::exists("entities/" + entityName + ".json"))
                return;

            DependencyList dList = levelLoader_.GenerateEntityDependencyList(entityName);
            resourceManager_.LoadDependencies(dList);
        }

        Transform spawnTransform;
        spawnTransform.position = camera_.transform_.position + camera_.transform_.GetForwardVector() * 20.0f;
        entityManager_.spawnList_.push_back({
            resourceManager_.GetEntityDescription(entityName),
            spawnTransform
        });
        return;
    }
}
