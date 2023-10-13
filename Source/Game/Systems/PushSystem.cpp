#include "PushSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/HitboxComponent.h"
#include "../Components/HurtboxComponent.h"
#include "../Components/PushboxComponent.h"
#include "../Components/TransformComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<PushboxComponent, TransformComponent>();
void GetPushes(
    EntityList& entities,
    ComponentList& components,
    PushList& pushes 
) {
    auto& pushboxComponent = components.Get<PushboxComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int a = 0; a < MAX_ENTITIES; a++) {
        const Entity& entityA = entities[a];
        if (!entityA.ShouldUpdate())
            continue;
        if (!entityA.MatchesKey(key))
            continue;
        
        const bool& sendA = pushboxComponent.pushbox[a].send;
        const bool& recieveA = pushboxComponent.pushbox[a].recieve;
        Transform& transformA = transformComponent.transform[a];
        Pushbox& pushboxA = pushboxComponent.pushbox[a];

        // TODO: Only check components within the same chunk
        for (int b = a + 1; b < MAX_ENTITIES; b++) {
            if (b == a)
                continue;
            const Entity& entityB = entities[b];
            if (!entityB.ShouldUpdate())
                continue;
            if (!entityB.MatchesKey(key))
                continue;

            const bool& sendB = pushboxComponent.pushbox[b].send;
            const bool& recieveB = pushboxComponent.pushbox[b].recieve;
            Transform& transformB = transformComponent.transform[b];
            Pushbox& pushboxB = pushboxComponent.pushbox[b];

            if ((!sendA && !sendB) || (!recieveA && !recieveB))
                continue;
            
            Push push = {a, b, Collision::GetCollision(transformA, pushboxA, transformB, pushboxB)};
            if (push.collision.isColliding)
                pushes.push_back(push);
        }
    }
}

void PushSystem::Execute(
    EntityList& entities,
    ComponentList& components
) {
    auto& pushboxComponent = components.Get<PushboxComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        pushboxComponent.sentPush[i] = false;
        pushboxComponent.recievedPush[i] = false;
    }

    PushList pushes;
    GetPushes(entities, components, pushes);

    for (Push& push : pushes) {
        const bool& sendA = pushboxComponent.pushbox[push.entityA].send;
        const bool& sendB = pushboxComponent.pushbox[push.entityB].send;
        const bool& recieveA = pushboxComponent.pushbox[push.entityA].recieve;
        const bool& recieveB = pushboxComponent.pushbox[push.entityB].recieve;

        if (sendA && recieveA && sendB && recieveB) {
            transformComponent.transform[push.entityA].position -= push.collision.resolution * 0.5f;
            transformComponent.transform[push.entityB].position += push.collision.resolution * 0.5f;
            pushboxComponent.sentPush[push.entityA] = true;
            pushboxComponent.sentPush[push.entityB] = true;
            pushboxComponent.recievedPush[push.entityA] = true;
            pushboxComponent.recievedPush[push.entityB] = true;
        }
        else if (sendA && recieveB) {
            transformComponent.transform[push.entityB].position += push.collision.resolution * 0.5f;
            pushboxComponent.sentPush[push.entityA] = true;
            pushboxComponent.recievedPush[push.entityB] = true;
        }
        else if (sendB && recieveA) {
            transformComponent.transform[push.entityA].position -= push.collision.resolution * 0.5f;
            pushboxComponent.sentPush[push.entityB] = true;
            pushboxComponent.recievedPush[push.entityA] = true;
        }
    }
}
