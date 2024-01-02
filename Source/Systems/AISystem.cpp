#include "AISystem.h"
#include "AI/AI.h"
#include "Components/AIComponent.h"

constexpr EntityKey key = GetEntityKey<AIComponent>();

void AISystem::InitAIs(
    EntityList& entities,
    ComponentList& components,
    Terrain& terrain
) {
    AIComponent& aiComponent = components.Get<AIComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_) continue;
        if (!entity.MatchesKey(key)) continue;
        if (aiComponent.timer[i] >= 0) continue;

        switch(aiComponent.controller[i]) {
            case AIComponent::AI_SpinRat:
                SpinRatController::Init(i, entities, components, terrain);
                break;

            case AIComponent::AI_Dummy:
                break;
        }
        aiComponent.timer[i] = 0;
    }
}

void AISystem::Execute(
    EntityList& entities,
    ComponentList& components,
    Terrain& terrain
) {
    AIComponent& aiComponent = components.Get<AIComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_) continue;
        if (!entity.MatchesKey(key)) continue;

        switch(aiComponent.controller[i]) {
            case AIComponent::AI_SpinRat:
                SpinRatController::Control(i, entities, components, terrain);
                break;

            case AIComponent::AI_Dummy:
                break;
        }
    }
}
