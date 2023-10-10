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
    std::array<Entity, MAX_ENTITIES>& entities,
    PushboxComponent& pushboxComponent,
    TransformComponent& transformComponent,
    PushList& pushes 
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity1 = entities[i];
        if (!entity1.ShouldUpdate())
            continue;
        if (!entity1.MatchesKey(key))
            continue;
        
        vec3& pos1 = transformComponent.transform[i].position;
        Pushbox& pushbox1 = pushboxComponent.pushbox[i];
        float scale1 = (transformComponent.transform[i].scale.x + transformComponent.transform[i].scale.z) * 0.5f;

        // TODO: Only check components within the same chunk
        for (int j = i + 1; j < MAX_ENTITIES; j++) {
            if (j == i)
                continue;
            const Entity& entity2 = entities[j];
            if (!entity2.ShouldUpdate())
                continue;
            if (!entity2.MatchesKey(key))
                continue;

            vec3& pos2 = transformComponent.transform[j].position;
            Pushbox& pushbox2 = pushboxComponent.pushbox[j];
            float scale2 = (transformComponent.transform[j].scale.x + transformComponent.transform[j].scale.z) * 0.5f;
            
            float dist = distance(pos1, pos2);
            vec3 direction = normalize(pos2 - pos1);
            float maxDist = pushbox1.radius * scale1 + pushbox2.radius * scale2;
            if (dist < maxDist) {
                vec3 resolutionVec = direction * (maxDist - dist);
                resolutionVec.y = 0.0f;
                pushes.push_back({i, j, resolutionVec}); 
            }
        }
    }
}

void PushSystem::Execute(
    std::array<Entity, MAX_ENTITIES>& entities,
    PushboxComponent& pushboxComponent,
    TransformComponent& transformComponent
) {
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
    GetPushes(entities, pushboxComponent, transformComponent, pushes);

    for (Push& push : pushes) {
        const bool& entity1Send = pushboxComponent.pushbox[push.entity1].send;
        const bool& entity2Send = pushboxComponent.pushbox[push.entity2].send;
        const bool& entity1Recieve = pushboxComponent.pushbox[push.entity1].recieve;
        const bool& entity2Recieve = pushboxComponent.pushbox[push.entity2].recieve;

        if (entity1Send && entity1Recieve && entity2Send && entity2Recieve) {
            transformComponent.transform[push.entity1].position -= push.resolutionVec * 0.5f;
            transformComponent.transform[push.entity2].position += push.resolutionVec * 0.5f;
            pushboxComponent.sentPush[push.entity1] = true;
            pushboxComponent.sentPush[push.entity2] = true;
            pushboxComponent.recievedPush[push.entity1] = true;
            pushboxComponent.recievedPush[push.entity2] = true;
        }
        else if (entity1Send && entity2Recieve) {
            transformComponent.transform[push.entity2].position += push.resolutionVec;
            pushboxComponent.sentPush[push.entity1] = true;
            pushboxComponent.recievedPush[push.entity2] = true;
        }
        else if (entity2Send && entity1Recieve) {
            transformComponent.transform[push.entity1].position -= push.resolutionVec;
            pushboxComponent.sentPush[push.entity2] = true;
            pushboxComponent.recievedPush[push.entity1] = true;
        }
    }
}
